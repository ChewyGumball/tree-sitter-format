#pragma once

#include <cassert>
#include <iterator>
#include <string_view>
#include <vector>

namespace tree_sitter_format {
    class UnicodeIterator : public std::iterator<std::forward_iterator_tag,
                                                 char32_t,
                                                 size_t,
                                                 const char32_t *,
                                                 char32_t
                                                >
    {
    private:
        const std::vector<std::string_view>& elements;
        
        size_t currentStartIndex;
        size_t currentStartOffset;
        size_t currentEndIndex;
        size_t currentEndOffset;

        char32_t currentValue;

    public:
        explicit UnicodeIterator(const std::vector<std::string_view>& elmenets);
        UnicodeIterator(const std::vector<std::string_view>& elements, size_t startIndex, size_t startOffset);

        UnicodeIterator(const UnicodeIterator& other) = default;

        size_t startIndex() const { return currentStartIndex; }
        size_t startOffset() const { return currentStartOffset; }
        size_t endIndex() const { return currentEndIndex; }
        size_t endOffset() const { return currentEndOffset; }

        UnicodeIterator& operator++(int);

        bool operator==(const UnicodeIterator& other) const;
        bool operator!=(const UnicodeIterator& other) const;

        std::strong_ordering operator<=>(const UnicodeIterator& other) const;

        char32_t operator*() const { return currentValue; }

        static std::vector<std::string_view> ElementsBetween(const UnicodeIterator& start, const UnicodeIterator& end);
    };
}
