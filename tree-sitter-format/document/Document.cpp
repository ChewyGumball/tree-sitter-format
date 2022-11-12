#include <tree-sitter-format/document/Document.h>

#include <tree-sitter-format/Constants.h>

#include <algorithm>

#include <fstream>
#include <sstream>

#include <assert.h>

namespace {
    std::string ReadFile(const std::filesystem::path& file) {
        std::ifstream in(file);
        std::ostringstream s;
        s << in.rdbuf();

        return s.str();
    }

    struct FormattableRange {
        std::vector<tree_sitter_format::Range> ranges;
        std::optional<TSNode> currentStartNode;
        const tree_sitter_format::Document& document;
    };

    void FindUnformattableRanges(TSTreeCursor* cursor, FormattableRange& r) {
        using namespace tree_sitter_format;
        using namespace std::string_view_literals;

        if (ts_tree_cursor_goto_first_child(cursor)) {
            do {
                FindUnformattableRanges(cursor, r);
            } while (ts_tree_cursor_goto_next_sibling(cursor));
            ts_tree_cursor_goto_parent(cursor);
        } else {
            TSNode node = ts_tree_cursor_current_node(cursor);
            if (ts_node_symbol(node) != COMMENT) {
                return;
            }

            DocumentSlice comment = r.document.slice(Range::Of(node)).trimBack();

            if (r.currentStartNode.has_value()) {
                if (comment.is("// clang-format: off"sv) || comment.is("// tree-sitter-format: off"sv)) {
                    Position start = Position::StartOf(r.currentStartNode.value());
                    Position end = Position::EndOf(node);
                    r.ranges.push_back(Range::Between(start, end));

                    r.currentStartNode = std::nullopt;
                }
            } else {
                if (comment.is("// clang-format: on"sv) || comment.is("// tree-sitter-format: on"sv)) {
                    r.currentStartNode = node;
                }
            }
        }
    }

    std::vector<tree_sitter_format::Range> FindUnformattableRanges(const tree_sitter_format::Document& document) {
        using namespace tree_sitter_format;

        FormattableRange r {
            .currentStartNode = std::nullopt,
            .document = document,
        };

        TSTreeCursor cursor = ts_tree_cursor_new(document.root());
        FindUnformattableRanges(&cursor, r);
        ts_tree_cursor_delete(&cursor);

        if (r.currentStartNode.has_value()) {
            Position start = Position::StartOf(r.currentStartNode.value());
            Position end = Position::EndOf(document.root());
            r.ranges.push_back(Range::Between(start, end));
        }

        return r.ranges;
    }
}

extern "C" {
const TSLanguage* tree_sitter_cpp(void);
}

namespace tree_sitter_format {

    size_t Document::splitAtPosition(uint32_t position) {
        assert(position <= length);

        if (position == length) {
            return elements.size();
        }

        // Find the element with the greatest start point before the split point
        Location element = findByteLocation(position);

        // If the split is in the middle of this element, split it in two at that point.
        if (element.offset > 0) {
            std::string_view remaining = elements[element.index].substr(element.offset);
            elements.insert(elements.begin() + element.index + 1, remaining);
            elements[element.index].remove_suffix(remaining.size());

            return element.index + 1;
        } else {
            return element.index;
        }
    }

    const char* Document::Read(void* payload, uint32_t byte_index, [[maybe_unused]]TSPoint position_unused, uint32_t *bytes_read) {
        Document* document = (Document*)payload;

        if (byte_index >= document->length) {
            *bytes_read = 0;
            return nullptr;
        }

        Location location = document->findByteLocation(byte_index);
        std::string_view element = document->elements[location.index];
        element.remove_prefix(location.offset);

        *bytes_read = uint32_t(element.size());
        return element.data();
    }

    Document::Document(const std::filesystem::path& file) : Document(ReadFile(file)) {}

    Document::Document(const std::string& contents) : Document(std::string(contents)) {}

    Document::Document(std::string&& contents) : original(std::move(contents)), parser(ts_parser_new(), ts_parser_delete), tree(nullptr, ts_tree_delete) {
        elements.emplace_back(original);
        length = original.size();

        ts_parser_set_language(parser.get(), tree_sitter_cpp());
        tree.reset(ts_parser_parse(parser.get(), nullptr, inputReader()));

        elementRange.end = Position::EndOf(root());

        unformattableRanges = FindUnformattableRanges(*this);
    }

    void Document::insertBytes(const Position& position, std::string_view bytes) {
        size_t insertPosition = splitAtPosition(position.byteOffset);
        elements.insert(elements.begin() + insertPosition, bytes);

        length += bytes.size();

        TSInputEdit edit {
            .start_byte = position.byteOffset,
            .old_end_byte = position.byteOffset,
            .new_end_byte = position.byteOffset + uint32_t(bytes.size()),
            .start_point = position.location,
            .old_end_point = position.location,
            .new_end_point = TSPoint{
                .row = position.location.row,
                .column = position.location.column + uint32_t(bytes.size()),
            },
        };

        ts_tree_edit(tree.get(), &edit);
    }

    void Document::deleteBytes(const Range& range) {
        size_t deletePosition = splitAtPosition(range.start.byteOffset);

        // Start deleting
        size_t bytesRemaining = range.byteCount();
        while(bytesRemaining > 0) {
            // If we were asked to delete past the end of the contents, just ignore it
            if (deletePosition >= elements.size()) {
                return;
            }

            std::string_view& element = elements[deletePosition];

            size_t bytesToDelete = std::min(bytesRemaining, element.size());
            bytesRemaining -= bytesToDelete;
            length -= bytesToDelete;

            element.remove_prefix(bytesToDelete);
            if (element.empty()) {
                elements.erase(elements.begin() + deletePosition);
            }
        }

        TSInputEdit edit {
            .start_byte = range.start.byteOffset,
            .old_end_byte = range.end.byteOffset,
            .new_end_byte = range.start.byteOffset,
            .start_point = range.start.location,
            .old_end_point = range.end.location,
            .new_end_point = range.start.location,
        };

        ts_tree_edit(tree.get(), &edit);
    }

    void Document::applyEdits(std::vector<Edit> edits) {
        std::ranges::sort(edits);

        for(const Edit& edit : edits) {
            if (std::holds_alternative<DeleteEdit>(edit)) {
                const DeleteEdit& d = std::get<DeleteEdit>(edit);
                deleteBytes(d.range);
            } else if (std::holds_alternative<InsertEdit>(edit)) {
                const InsertEdit& i = std::get<InsertEdit>(edit);
                insertBytes(i.position, i.bytes);
            }
        }

        tree.reset(ts_parser_parse(parser.get(), tree.release(), inputReader()));
        elementRange.end = Position::EndOf(root());
        // TODO delete old tree or no?

        unformattableRanges = FindUnformattableRanges(*this);
    }

    const std::string& Document::originalContents() const {
        return original;
    }

    const std::string_view Document::originalContentsAt(const Range& range) const {
        return std::string_view(original).substr(range.start.byteOffset, range.byteCount());
    }

    bool Document::overlapsUnformattableRange(const Range& range) const {
        for(const Range& unformattableRange : unformattableRanges) {
            if (unformattableRange.start >= range.end) {
                break;
            }

            if (unformattableRange.end > range.start) {
                return true;
            }
        }

        return false;
    }

    TSNode Document::root() const {
        return ts_tree_root_node(tree.get());
    }

    TSInput Document::inputReader() {
        return TSInput {
            .payload = this,
            .read = &Document::Read,
            .encoding = TSInputEncodingUTF8,
        };
    }

}
