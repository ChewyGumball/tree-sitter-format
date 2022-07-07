#include <tree_sitter_format/document/Document.h>

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
    Document::BytePosition Document::findBytePosition(uint32_t position) const {
        assert(position < length);

        for(size_t i = 0, currentLength = 0; i < elements.size(); i++) {
            size_t nextSize = elements[i].size();

            if (currentLength + nextSize > position) {
                return BytePosition {
                    .index = i,
                    .offset = position - currentLength,
                };
            }

            currentLength += nextSize;
        }

        assert(false);
        // We should never get here
        return BytePosition();
    }

    size_t Document::splitAtPosition(uint32_t position) {
        assert(position <= length);

        if (position == length) {
            return elements.size();
        }

        // Find the element with the greatest start point before the split point        
        BytePosition element = findBytePosition(position);

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

        BytePosition position = document->findBytePosition(byte_index);
        std::string_view element = document->elements[position.index];
        element.remove_prefix(position.offset);

        *bytes_read = uint32_t(element.size());
        return element.data();
    }

    Document::Document(const std::filesystem::path& file) : Document(ReadFile(file)) {}
    Document::Document(const std::string&& contents) : original(std::move(contents)), elements({original}), length(original.size()), parser(ts_parser_new(), ts_parser_delete), tree(nullptr, ts_tree_delete) {
        ts_parser_set_language(parser.get(), tree_sitter_cpp());
        tree.reset(ts_parser_parse(parser.get(), nullptr, inputReader()));
    }

    void Document::insertBytes(uint32_t position, std::string_view bytes) {
        size_t insertPosition = splitAtPosition(position);
        elements.insert(elements.begin() + insertPosition, bytes);

        length += bytes.size();
    }

    void Document::deleteBytes(Range range) {
        size_t deletePosition = splitAtPosition(range.start);
        
        // Start deleting
        size_t bytesRemaining = range.count();
        while(bytesRemaining > 0) {
            // If we were asked to delete past the end of the contents, just ignore it
            if (deletePosition >= elements.size()) {
                return;
            }

            std::string_view element = elements[deletePosition];

            size_t bytesToDelete = std::min(bytesRemaining, element.size());
            bytesRemaining -= bytesToDelete;
            length -= bytesToDelete;

            element.remove_prefix(bytesToDelete);
            if (element.empty()) {
                elements.erase(elements.begin() + deletePosition);
            } 
        }
    }

    const std::string& Document::originalContents() const {
        return original;
    }
    
    const std::string_view Document::originalContentsAt(Range range) const {
        return std::string_view(original).substr(range.start, range.count());
    }
    
    TSInput Document::inputReader() {
        return TSInput {
            .payload = this,
            .read = &Document::Read,
            .encoding = TSInputEncodingUTF8,
        };
    }

    std::ostream& operator<<(std::ostream& out, const Document& document) {
        for(const std::string_view& e : document.elements) {
            out << e;
        }

        return out;
    }

}
