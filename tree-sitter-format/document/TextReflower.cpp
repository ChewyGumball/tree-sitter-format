#include <tree-sitter-format/document/TextReflower.h>

#include <tree-sitter-format/Util.h>

#include <array>

using namespace std::string_view_literals;

namespace {
    using namespace tree_sitter_format;

    [[nodiscard]] bool StartsWithPunctuation(const DocumentSlice& line) {
        UnicodeIterator it = line.begin();
        if (!IsPunctuation(*it)) {
            return false;
        }

        ++it;

        if (!IsPunctuation(*it)) {
            return false;
        }

        return true;
    }

    [[nodiscard]] bool StartsWithWhitespace(const DocumentSlice& line) {
        return line.startsWith("   "sv) || line.startsWith("  \t") || line.startsWith(" \t") || line.startsWith("\t");
    }

    [[nodiscard]] bool StartsWithSpecialPrefix(const DocumentSlice& line) {
        static std::array<std::string_view, 5> specialPrefixes = {
            "@"sv,
            "TODO"sv,
            "FIXME"sv,
            "- "sv,
            "* "sv,
        };

        for(const auto& prefix : specialPrefixes) {
            if (line.startsWith(prefix)) {
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] std::optional<ListItemInfo> GetListItemInfo(const DocumentSlice& line) {
        UnicodeIterator it = line.begin();
        if (IsDigit(*it)) {
            ++it;
            if (*it == '.') {
                ++it;

                return ListItemInfo {
                    .number = line.slice(Range::Between(line.startPosition(), it.currentPosition())),
                    .columnCount = 2,
                };
            }

            if (IsDigit(*it)) {
                ++it;

                if (*it == '.') {
                    ++it;

                    return ListItemInfo {
                        .number = line.slice(Range::Between(line.startPosition(), it.currentPosition())),
                        .columnCount = 3,
                    };
                }
            }
        }

        return std::nullopt;
    }

    
    struct Word {
        std::vector<std::string_view> elements;
        uint32_t length;
    };

    std::vector<Word> SeparateWords(const DocumentSlice& line) {
        std::vector<Word> words;

        UnicodeIterator character = line.begin();
        UnicodeIterator end = line.end();

        while(character != end) {
            while(character != end && IsWhitespace(*character)) {
                character++;
            }

            if (character == end) {
                // We only have whitespace left on the line, so no more words
                break;
            }

            // byte count is not the same as column count, so we have to keep
            // track of it separately.
            uint32_t length = 0;
            UnicodeIterator start = character;

            while(character != end && !IsWhitespace(*character)) {
                character++;
                length++;
            }

            words.push_back(Word{
                .elements = UnicodeIterator::ElementsBetween(start, character),
                .length = length,
            });
        }

        return words;
    }
}

namespace tree_sitter_format {

    LineInfo::LineInfo(const DocumentSlice& line) {
        DocumentSlice trimmed = line.trimFront();

        isEmpty = trimmed.range().byteCount() == 0;
        startsWithPunctuation = StartsWithPunctuation(trimmed);
        startsWithWhitespace = StartsWithWhitespace(line);
        startsWithSpecialPrefix = StartsWithSpecialPrefix(line);
        listItemInfo = GetListItemInfo(trimmed);
        initialWhitespace = Range::Between(line.startPosition(), trimmed.startPosition());
        endColumn = line.endPosition().location.column;
    }


    TextReflower::TextReflower(uint32_t startingLineLength, uint32_t targetLineLength)
     : linePrefix(" * "sv), newLineIndent(0), targetLineLength(targetLineLength)
     , currentLineLength(0), currentWordCount(0), startingLineLength(startingLineLength) {
        // The first line always starts with a space right after the opening comment token.
        // The line prefix is only added to subsequent lines.
        currentLine.push_back(GetSpaces(1));
        currentLineLength = startingLineLength + 1;
    }

    void TextReflower::addWords(const DocumentSlice& line) {
        std::vector<Word> words = SeparateWords(line);
        for(const Word& word : words) {
            // If adding the next word would cause the current line to go past the target line length,
            // record the line, then start a new line. Also, we want at least one word per line, so
            // if the current line is empty, don't start a new line yet.
            if (currentLineLength + word.length > targetLineLength && currentWordCount > 0) {
                startNewLine();
            }

            // Add the next word to the line
            currentLine.insert(currentLine.end(), word.elements.begin(), word.elements.end());
            currentLineLength += word.length;
            currentWordCount++;

            // Add a space to the line
            currentLine.push_back(" "sv);
            currentLineLength++;
        }
    }

    void TextReflower::addWhitespace(const DocumentSlice& whitespace) {
        auto& contents = whitespace.contents();
        currentLine.insert(currentLine.end(), contents.begin(), contents.end());
        currentLineLength += whitespace.range().end.location.column - whitespace.range().start.location.column;
        // Since this is just white space (we take the caller's word for it), we don't increment the word count
        // because we want it to be trimmed if we start a new line without adding anything more.
    }

    void TextReflower::addLineAsIs(const DocumentSlice& line) {
        auto& contents = line.contents();
        currentLine.insert(currentLine.end(), contents.begin(), contents.end());
        currentLineLength += line.range().end.location.column - line.range().start.location.column;

        // Add a space to the line
        currentLine.push_back(" "sv);
        currentLineLength++;

        // We count this as a single word because we don't know how many real words there are, but
        // we need to record that some were added, otherwise other functions may thing the line is
        // empty.
        currentWordCount++;
    }

    void TextReflower::addEmptyLine() {
        setIndentation(0);
        startNewLineIfNotEmpty();

        startNewLine();
    }

    void TextReflower::startNewLine() {
        // Remove any spaces at the end of the line. We don't have to 
        // worry about maintaining the line length since we will be
        // clearing it out immediately after.
        std::string_view& last = currentLine.back();
        while(last.ends_with(' ') || last.ends_with('\t')) {
            last.remove_suffix(1);
            if (last.empty()) {
                currentLine.pop_back();

                assert(!currentLine.empty());
                last = currentLine.back();
            }
        }

        // Record the current line
        lines.push_back(currentLine);

        // Start a new line
        currentLine.clear();
        currentLineLength = 0;

        currentLine.push_back(linePrefix);
        currentLineLength += uint32_t(linePrefix.size());

        if (newLineIndent > 0) {
            currentLine.push_back(GetSpaces(newLineIndent));
            currentLineLength += newLineIndent;
        }

        currentWordCount = 0;
    }

    void TextReflower::startNewLineIfNotEmpty() {
        if (currentWordCount > 0) {
            startNewLine();
        }
    }

    void TextReflower::setIndentation(uint32_t newIndentation) {
        // If this is an empty line we can just redo the indentation
        if (currentWordCount == 0) {
            currentLine.clear();
            currentLineLength = 0;

            if (!lines.empty()) {
                currentLine.push_back(linePrefix);
                currentLineLength += uint32_t(linePrefix.size());
            } else {
                currentLineLength = startingLineLength;
            }

            currentLine.push_back(GetSpaces(newIndentation));
            currentLineLength += newIndentation;
        }

        newLineIndent = newIndentation;
    }

    void TextReflower::startNewListItem(const ListItemInfo& info) {
        // List items always start 2 spaces indented
        setIndentation(2);
        startNewLineIfNotEmpty();

        // Add the number
        auto& number = info.number.contents();
        currentLine.insert(currentLine.end(), number.begin(), number.end());
        currentLineLength += info.columnCount;
        currentWordCount++;

        // Add a space after the number
        currentLine.push_back(GetSpaces(1));
        currentLineLength++;

        // subsequent lines are indented more, this is + 1 because we
        // want to aling the text after the space after the period
        setIndentation(2 + info.columnCount + 1);
    }

    void TextReflower::endListItem() {
        setIndentation(0);
    }

    void TextReflower::startNewParagraph() {
        // Paragraphs have no extra indentation
        setIndentation(0);
        startNewLineIfNotEmpty();
    }
}
