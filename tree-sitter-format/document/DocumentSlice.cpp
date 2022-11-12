#include <tree-sitter-format/document/DocumentSlice.h>

#include <tree-sitter-format/Util.h>

#include <cassert>
#include <optional>
#include <sstream>

namespace tree_sitter_format {
    Location DocumentSlice::findByteLocation(uint32_t position) const {
        assert(position >= elementRange.start.byteOffset);

        uint32_t relativePosition = position - elementRange.start.byteOffset;
        assert(relativePosition < length);

        for(size_t i = 0, currentLength = 0; i < elements.size(); i++) {
            size_t nextSize = elements[i].size();

            if (currentLength + nextSize > relativePosition) {
                return Location {
                    .index = i,
                    .offset = relativePosition - currentLength,
                };
            }

            currentLength += nextSize;
        }

        assert(false);
        // We should never get here
        return Location();
    }

    DocumentSlice::DocumentSlice(const Range& range, std::vector<std::string_view> elements)
     : elementRange(range), elements(std::move(elements)) {
        assert(!this->elements.empty());

        for(const std::string_view& element : this->elements) {
            length += element.size();
        }
    }

    DocumentSlice DocumentSlice::slice(const Range& subRange) const {
        return DocumentSlice(subRange, contentsAt(subRange));
    }


    DocumentSlice DocumentSlice::trimFront() const {
        UnicodeIterator front = begin();
        UnicodeIterator back = end();
        while(front != back && IsWhitespace(*front)) {
            front++;
        }

        Range subRange {
            .start = front.currentPosition(),
            .end = elementRange.end,
        };

        return slice(subRange);
    }

    DocumentSlice DocumentSlice::trimBack() const {
        UnicodeIterator front = begin();
        UnicodeIterator back = end();
        std::optional<Position> lastStartOfWhitespace = std::nullopt;
        bool lastWasWhitespace = false;

        while(front != back) {
            bool isWhitespace = IsWhitespace(*front);
            if (!isWhitespace) {
                lastStartOfWhitespace = std::nullopt;
            } else if (!lastWasWhitespace) {
                lastStartOfWhitespace = front.currentPosition();
            }

            lastWasWhitespace = isWhitespace;
            ++front;
        }

        Range subRange {
            .start = lastStartOfWhitespace.value_or(elementRange.end),
            .end = elementRange.end,
        };

        return slice(subRange);
    }

    std::vector<std::string_view> DocumentSlice::contentsAt(Range subRange) const {
        std::vector<std::string_view> contents;

        // If the subrange is empty, just return an empty contents.
        if (subRange.start == subRange.end) {
            contents.push_back(std::string_view());
            return contents;
        }

        Location location = findByteLocation(subRange.start.byteOffset);

        size_t index = location.index;
        uint32_t remaining = subRange.byteCount();

        std::string_view element = elements[index++];
        element.remove_prefix(location.offset);
        if (subRange.byteCount() < element.size()) {
            element.remove_suffix(element.size() - subRange.byteCount());
        }
        contents.push_back(element);

        remaining -= uint32_t(element.size());
        while (remaining > 0) {
            element = elements[index++];
            if (remaining > element.size()) {
                element.remove_suffix(element.size() - remaining);
            }
            contents.push_back(element);
            remaining -= uint32_t(element.size());
        }

        return contents;
    }

    std::string DocumentSlice::contentsAtAsString(Range subRange) const {
        std::ostringstream s;

        Location location = findByteLocation(subRange.start.byteOffset);

        size_t index = location.index;
        uint32_t remaining = subRange.byteCount();

        std::string_view element = elements[index++];
        element.remove_prefix(location.offset);
        if (subRange.byteCount() < element.size()) {
            element.remove_suffix(element.size() - subRange.byteCount());
        }
        s << element;

        remaining -= uint32_t(element.size());
        while (remaining > 0) {
            element = elements[index++];
            if (remaining > element.size()) {
                element.remove_suffix(element.size() - remaining);
            }

            s << element;
            remaining -= uint32_t(element.size());
        }

        return s.str();
    }

    char DocumentSlice::characterAt(uint32_t bytePosition) const {
        Location location = findByteLocation(bytePosition);
        return elements[location.index][location.offset];
    }

    bool DocumentSlice::startsWith(std::string_view bytes) const {
        size_t charactersLookedAt = 0;
        for(const std::string_view& element : elements) {
            for(char character : element) {
                if (character != bytes[charactersLookedAt]) {
                    return false;
                }

                charactersLookedAt++;

                if (charactersLookedAt == bytes.size()) {
                    return true;
                }
            }
        }

        return false;
    }

    bool DocumentSlice::is(std::string_view bytes) const {
        return bytes.size() == elementRange.byteCount() && startsWith(bytes);
    }

    Range DocumentSlice::nextNewLine(Position start) const {
        Location location = findByteLocation(start.byteOffset);

        static constexpr char8_t UTF8ContinuationMask = 0b11000000;
        static constexpr char8_t UTF8ContinuationValue = 0b10000000;

        uint32_t column = start.location.column;
        uint32_t byteOffset = start.byteOffset;

        std::optional<Position> newLineStart;

        bool previousCharacterWasEscape = false;

        for (size_t index = location.index; index < elements.size(); index++) {
            size_t startOffset = index == location.index ? location.offset : 0;

            for (size_t offset = startOffset; offset < elements[index].size(); offset++) {
                char character = elements[index][offset];

                if (newLineStart.has_value()) {
                    if (character != '\n') {
                        Position newLineEnd {
                            .location = TSPoint {
                                .row = start.location.row + 1,
                                .column = 0,
                            },
                            .byteOffset = byteOffset,
                        };

                        return Range {
                            .start = newLineStart.value(),
                            .end = newLineEnd,
                        };
                    }
                }

                char masked = character & UTF8ContinuationMask;
                bool isContinuation = masked == UTF8ContinuationValue;

                if (!isContinuation) {
                    bool isVerticalWhitespace = character == '\r' || character == '\n';
                    if (!previousCharacterWasEscape && isVerticalWhitespace) {
                        newLineStart = Position {
                            .location = TSPoint {
                                .row = start.location.row,
                                .column = column,
                            },
                            .byteOffset = byteOffset,
                        };

                        // If the character is a line feed, we are done. If it is
                        // a carriage return, we need to keep going to find the
                        // line feed character.
                        if (character == '\n') {
                            Position newLineEnd {
                                .location = TSPoint {
                                    .row = start.location.row + 1,
                                    .column = 0,
                                },
                                .byteOffset = byteOffset + 1,
                            };

                            return Range {
                                .start = newLineStart.value(),
                                .end = newLineEnd,
                            };
                        }
                    }

                    previousCharacterWasEscape = !previousCharacterWasEscape && character == '\\';
                    column++;
                }
                byteOffset++;
            }
        }

        Position endOfFilePosition {
            .location = TSPoint {
                .row = start.location.row,
                .column = column,
            },
            .byteOffset = byteOffset,
        };

        return Range {
            .start = endOfFilePosition,
            .end = endOfFilePosition,
        };
    }

    Range DocumentSlice::toNextNewLine(Position start) const {
        Location location = findByteLocation(start.byteOffset);

        Range newLine = nextNewLine(start);

        return Range {
            .start = start,
            .end = newLine.start,
        };
    }

    Range DocumentSlice::toPreviousNewLine(Position end) const {
        Location location = findByteLocation(end.byteOffset);

        static constexpr char8_t UTF8ContinuationMask = 0b11000000;
        static constexpr char8_t UTF8ContinuationValue = 0b10000000;

        uint32_t column = end.location.column;
        uint32_t byteOffset = end.byteOffset;

        while (column > 0) {
            char character = elements[location.index][location.offset];

            char masked = character & UTF8ContinuationMask;
            bool isContinuation = masked == UTF8ContinuationValue;

            if (!isContinuation) {
                column--;
            }
            byteOffset--;

            if (location.offset == 0) {
                if (location.index > 0) {
                    location.index--;
                    location.offset = elements[location.index].size() - 1;
                }
                else {
                    assert(column == 0);
                }
            }
        }

        Position start {
            .location = TSPoint {
                .row = end.location.row,
                .column = column,
            },
            .byteOffset = byteOffset,
        };

        return Range {
            .start = start,
            .end = end,
        };
    }


    UnicodeIterator DocumentSlice::begin() const {
        return UnicodeIterator(elements, elementRange);
    }
    UnicodeIterator DocumentSlice::end() const {
        Location end {
            .index = elements.size() - 1,
            .offset = elements.back().size(),
            .position = elementRange.end,
        };

        return UnicodeIterator(elements, end, end);
    }

    std::string DocumentSlice::toString() const {
        std::ostringstream s;
        s << *this;
        return s.str();
    }

    std::ostream& operator<<(std::ostream& out, const DocumentSlice& document) {
        for(const std::string_view& e : document.contents()) {
            out << e;
        }

        return out;
    }
}
