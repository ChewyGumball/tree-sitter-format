#pragma once

#include <tree-sitter-format/document/Position.h>
#include <tree-sitter-format/document/Range.h>

#include <cassert>
#include <compare>
#include <iterator>
#include <string_view>
#include <vector>

namespace tree_sitter_format {
    struct Location {
        size_t index;
        size_t offset;

        Position position;

        // Note: these are comparing the location within a vector of
        //       elements, NOT the position
        bool operator==(const Location& other) const;
        std::weak_ordering operator<=>(const Location& other) const;
    };

    class UnicodeIterator {
    private:
        const std::vector<std::string_view>& elements;

        Location current;
        Location next;
        Location end;

        char32_t currentValue;

    public:
        explicit UnicodeIterator(const std::vector<std::string_view>& elements);
        UnicodeIterator(const std::vector<std::string_view>& elements, const Range& range);
        UnicodeIterator(const std::vector<std::string_view>& elements, const Location& start, const Location& end);

        // Don't allow temporary element vectors since we only store a reference to them. This object doesn't own the elements
        // just iterates over them.
        explicit UnicodeIterator(const std::vector<std::string_view>&& elements) = delete;
        UnicodeIterator(const std::vector<std::string_view>&& elements, const Range& range) = delete;
        UnicodeIterator(const std::vector<std::string_view>&& elements, const Location& start, const Location& end) = delete;

        UnicodeIterator(const UnicodeIterator& other) = default;

        const Position& currentPosition() const { return current.position; }
        bool atEnd() const { return current == end; }

        UnicodeIterator operator++(int);
        UnicodeIterator& operator++();

        bool operator==(const UnicodeIterator& other) const;
        bool operator!=(const UnicodeIterator& other) const;

        std::strong_ordering operator<=>(const UnicodeIterator& other) const;

        char32_t operator*() const { return currentValue; }

        static std::vector<std::string_view> ElementsBetween(const UnicodeIterator& start, const UnicodeIterator& end);
    };
}
