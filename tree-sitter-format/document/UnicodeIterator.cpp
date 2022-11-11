#include <tree-sitter-format/document/UnicodeIterator.h>

namespace tree_sitter_format {

    bool Location::operator==(const Location& other) const {
        return index == other.index && offset == other.offset;
    }
    
    std::weak_ordering Location::operator<=>(const Location& other) const {
        if (index == other.index) {
            return offset <=> other.offset;
        } else {
            return index <=> other.index;
        }
    }

    UnicodeIterator::UnicodeIterator(const std::vector<std::string_view>& elements) : elements(elements), currentValue(0) {
        assert(!elements.empty());

        current = Location {
            .index = 0,
            .offset = 0,
            .position = Position {
                .location = TSPoint {
                    .row = 0,
                    .column = 0,
                },
                .byteOffset = 0,
            },
        };

        next = current;

        uint32_t totalBytes = 0;
        for(std::string_view element : elements) {
            totalBytes += uint32_t(element.size());
        }

        // We don't know the end location, but thats ok. It is never used for anything,
        // so it doesn't matter what we initialize it to. We do know the other, necessary
        // information though.
        end = Location {
            .index = elements.size() - 1,
            .offset = elements.back().size(),
            .position = Position {
                .location = TSPoint {
                    .row = 0,
                    .column = 0,
                },
                .byteOffset = totalBytes,
            },
        };

        (*this)++;
    }

    UnicodeIterator::UnicodeIterator(const std::vector<std::string_view>& elements, const Range& range) : elements(elements), currentValue(0) {
        assert(!elements.empty());

        current = Location {
            .index = 0,
            .offset = 0,
            .position = range.start,
        };

        next = current;

        end = Location {
            .index = elements.size() - 1,
            .offset = elements.back().size(),
            .position = range.end,
        };

        (*this)++;
    }

    UnicodeIterator::UnicodeIterator(const std::vector<std::string_view>& elements, const Location& start, const Location& end) 
        : elements(elements), current(start), next(start), end(std::move(end)), currentValue(0) { 
        assert(!elements.empty());
        
        (*this)++;
    }

    UnicodeIterator UnicodeIterator::operator++(int) {
        UnicodeIterator original = *this;
        ++(*this);

        return original;
    }

    UnicodeIterator& UnicodeIterator::operator++() {
        static constexpr char8_t UTF8ContinuationMask = 0b11000000;
        static constexpr char8_t UTF8ContinuationValue = 0b10000000;

        // If we are at the end, do nothing
        if (current == end) {
            return *this;
        }

        current = next;
        char32_t newValue = 0;

        uint32_t byteCount = 0;

        for(; next.index <= end.index; next.index++) {
            size_t maxOffset = elements[next.index].size();
            if (next.index == end.index) {
                maxOffset = end.offset;
            }

            for(size_t offset = next.offset; offset < maxOffset; next.offset++) {
                char32_t character = elements[next.index][next.offset];

                char masked = character & UTF8ContinuationMask;
                bool isContinuation = masked == UTF8ContinuationValue;

                newValue |= character << 24;
                newValue >>= 8;
                byteCount++;

                // we shouldn't have more than 3 continuation bytes, otherwise the
                // character doesn't fit in 32 bits which shouldn't be possible.
                assert(byteCount < 4 || !isContinuation);

                if (!isContinuation) {
                    next.offset++;

                    newValue >>= 8 * (3 - byteCount);

                    if (currentValue != '\\' && newValue == '\n') {
                        next.position.location.row++;
                        next.position.location.column = 0;
                    } else {
                        next.position.location.column++;
                    }

                    next.position.byteOffset += byteCount;
                    
                    currentValue = newValue;
                    return *this;
                }
            }
        }
        // If we reach here, its because we just iterated to the end, which means the start should
        // equal the end, which is one past the end of the last character in the last element.
        assert(current == end);

        return *this;
    }

    bool UnicodeIterator::operator==(const UnicodeIterator& other) const {
        return (&elements == &other.elements) && ((*this <=> other) == std::strong_ordering::equal);
    }

    bool UnicodeIterator::operator!=(const UnicodeIterator& other) const {
        return (&elements != &other.elements) || !(*this == other);
    }

    std::strong_ordering UnicodeIterator::operator<=>(const UnicodeIterator& other) const {
        // Both iterators should be pointing at the same elements
        assert(&elements == &other.elements);
        
        if (current.index < other.current.index) {
            return std::strong_ordering::less;
        }

        if (current.index > other.current.index) {
            return std::strong_ordering::greater;
        }

        if (current.offset < other.current.offset) {
            return std::strong_ordering::less;
        }

        if (current.offset > other.current.offset) {
            return std::strong_ordering::greater;
        }

        return std::strong_ordering::equal;
    }
    
    std::vector<std::string_view> UnicodeIterator::ElementsBetween(const UnicodeIterator& start, const UnicodeIterator& end) {
        // Make sure the end doesn't come before the start
        assert(start <= end);

        std::vector<std::string_view> elements;
        for(size_t index = start.current.index; index <= end.current.index; index++) {
            std::string_view element = start.elements[index];

            // Remove the end first, otherwise the element would have a different length than
            // when the offset was recorded, making it invalid.
            if (index == end.current.index) {
                element.remove_suffix(element.size() - end.current.offset);
            }

            if (index == start.current.index) {
                element.remove_prefix(start.current.offset);
            }

            // We may have a zero size element if we are given the same iterators, or when the end
            // iterator is the start of an element. In that case we don't wan't to record it.
            if (element.size() > 0) {
                elements.push_back(element);
            }
        }
        
        return elements;
    }
}
