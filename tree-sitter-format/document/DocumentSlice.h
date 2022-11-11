#pragma once

#include <string_view>
#include <vector>

#include <tree-sitter-format/document/Position.h>
#include <tree-sitter-format/document/Range.h>
#include <tree-sitter-format/document/UnicodeIterator.h>

namespace tree_sitter_format {

class DocumentSlice {
protected:
    Range elementRange;
    std::vector<std::string_view> elements;
    size_t length;

    DocumentSlice() = default;

    Location findByteLocation(uint32_t position) const;

public:
    

    DocumentSlice(const Range& range, std::vector<std::string_view> elements);

    const Position& startPosition() const { return elementRange.start; }
    const Position& endPosition() const { return elementRange.end; }
    const Range& range() const { return elementRange; }

    DocumentSlice slice(const Range& subRange) const;
    DocumentSlice trimFront() const;

    const std::vector<std::string_view>& contents() const { return elements;}
    std::vector<std::string_view> contentsAt(Range subRange) const;
    std::string contentsAtAsString(Range subRange) const;

    char characterAt(uint32_t bytePosition) const;

    bool startsWith(std::string_view bytes) const;

    Range nextNewLine(Position start) const;

    Range toNextNewLine(Position start) const;
    Range toPreviousNewLine(Position end) const;

    UnicodeIterator begin() const;
    UnicodeIterator end() const;

    std::string toString() const;
};

std::ostream& operator<<(std::ostream& out, const DocumentSlice& document);

}
