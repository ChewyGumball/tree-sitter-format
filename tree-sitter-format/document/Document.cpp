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

namespace tree_sitter_format {
    
    std::optional<Document::BytePosition> Document::findBytePosition(uint32_t position) const {
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

        return std::nullopt;
    }

    void Document::splitAtPosition(uint32_t position) {
        // Find the element with the greatest start point before the split point        
        std::optional<BytePosition> element = findBytePosition(position);
        assert(element.has_value());

        // If the split is in the middle of this element, split it in two at that point.
        if (element->offset > 0) {
            std::string_view remaining = elements[element->index].substr(element->offset);
            elements.insert(elements.begin() + element->index + 1, remaining);
            elements[element->index].remove_suffix(remaining.size());
        }
    }
    
    const char* Document::Read(void* payload, uint32_t byte_index, [[maybe_unused]]TSPoint position_unused, uint32_t *bytes_read) {
        Document* document = (Document*)payload;

        std::optional<BytePosition> position = document->findBytePosition(byte_index);

        if (position.has_value()) {
            std::string_view element = document->elements[position->index];
            element.remove_prefix(position->offset);

            *bytes_read = uint32_t(element.size());
            return element.data();
        } else {
            *bytes_read = 0;
            return nullptr;
        }
    }

    Document::Document(const std::filesystem::path& file) : Document(ReadFile(file)) {}
    Document::Document(const std::string&& contents) : original(std::move(contents)), elements({original}) {}

    void Document::insertBytes(uint32_t position, std::string_view bytes) {
        splitAtPosition(position);

        // Insert the new bytes
        std::optional<BytePosition> element = findBytePosition(position);
        assert(element.has_value());
        assert(element->offset == 0);
        elements.insert(elements.begin() + element->index, bytes);
    }

    void Document::deleteBytes(Range range) {
        splitAtPosition(range.start);
        
        // Start deleting
        size_t bytesRemaining = range.count();
        while(bytesRemaining > 0) {  
            std::optional<BytePosition> elementPosition = findBytePosition(range.start);

            // If we were asked to delete past the end of the contents, just ignore it
            if (!elementPosition.has_value()) {
                return;
            }

            assert(elementPosition->offset == 0);
            std::string_view element = elements[elementPosition->index];

            size_t bytesToDelete = std::min(bytesRemaining, element.size());
            bytesRemaining -= bytesToDelete;

            element.remove_prefix(bytesToDelete);
            if (element.empty()) {
                elements.erase(elements.begin() + elementPosition->index);
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
