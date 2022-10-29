#include <tree_sitter_format/document/UnicodeIterator.h>

namespace tree_sitter_format {
    UnicodeIterator::UnicodeIterator(const std::vector<std::string_view>& elements)
        : UnicodeIterator(elements, 0, 0) { }

    UnicodeIterator::UnicodeIterator(const std::vector<std::string_view>& elements, size_t startIndex, size_t startOffset)
        : elements(elements),
          currentStartIndex(startIndex),
          currentStartOffset(startOffset),
          currentEndIndex(startIndex),
          currentEndOffset(startOffset),
          currentValue(0) {
            (*this)++;
    }

    UnicodeIterator& UnicodeIterator::operator++(int) {
        static constexpr char8_t UTF8ContinuationMask = 0b11000000;
        static constexpr char8_t UTF8ContinuationValue = 0b10000000;

        // If we are at the end, do nothing
        if (currentStartIndex == elements.size()) {
            return *this;
        }

        currentStartIndex = currentEndIndex;
        currentStartOffset = currentEndOffset;
        currentValue = 0;

        size_t byteCount = 0;

        for(size_t index = currentStartIndex; index < elements.size(); index++) {
            for(size_t offset = currentStartOffset; offset < elements[index].size(); offset++) {
                char32_t character = elements[index][offset];

                char masked = character & UTF8ContinuationMask;
                bool isContinuation = masked == UTF8ContinuationValue;

                currentValue |= character << 24;
                currentValue >>= 8;
                byteCount++;

                // we shouldn't have more than 3 continuation bytes, otherwise the
                // character doesn't fit in 32 bits which shouldn't be possible.
                assert(byteCount < 4 || !isContinuation);

                if (!isContinuation) {
                    currentEndIndex = index;
                    currentEndOffset = offset + 1;
                    currentValue >>= 8 * (3 - byteCount);

                    return *this;
                }
            }
        }
        // If we reach here, its because we just iterated to the end, which means the start should
        // equal the end, which is one past the end of the last character in the last element.
        assert(currentStartIndex == currentEndIndex);
        assert(currentStartOffset == currentEndOffset);
        assert(currentStartIndex == elements.size() - 1);
        assert(currentStartOffset == elements.back().size());

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
        
        if (currentStartIndex < other.currentStartIndex) {
            return std::strong_ordering::less;
        }

        if (currentStartIndex > other.currentStartIndex) {
            return std::strong_ordering::greater;
        }

        if (currentStartOffset < other.currentStartOffset) {
            return std::strong_ordering::less;
        }

        if (currentStartOffset > other.currentStartOffset) {
            return std::strong_ordering::greater;
        }

        return std::strong_ordering::equal;
    }
    
    std::vector<std::string_view> UnicodeIterator::ElementsBetween(const UnicodeIterator& start, const UnicodeIterator& end) {
        // Make sure the end doesn't come before the start
        assert(start <= end);

        std::vector<std::string_view> elements;
        for(size_t index = start.currentStartIndex; index <= end.currentStartIndex; index++) {
            std::string_view element = start.elements[index];

            // Remove the end first, otherwise the element would have a different length than
            // when the offset was recorded, making it invalid.
            if (index == end.currentStartIndex) {
                element.remove_suffix(element.size() - end.currentStartOffset);
            }

            if (index == start.currentStartIndex) {
                element.remove_prefix(start.currentStartOffset);
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
