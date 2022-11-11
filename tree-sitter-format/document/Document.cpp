#include <tree-sitter-format/document/Document.h>

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
    }

    void Document::insertBytes(Position position, std::string_view bytes) {
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

    void Document::deleteBytes(Range range) {
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
    }

    const std::string& Document::originalContents() const {
        return original;
    }
    
    const std::string_view Document::originalContentsAt(Range range) const {
        return std::string_view(original).substr(range.start.byteOffset, range.byteCount());
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
