#pragma once

#include <tree_sitter_format/document/DocumentSlice.h>

#include <optional>
#include <string_view>
#include <vector>


namespace tree_sitter_format {

    struct ListItemInfo {
        DocumentSlice number;
        uint32_t columnCount;
    };

    struct LineInfo {
        bool isEmpty;
        bool startsWithPunctuation;
        bool startsWithWhitespace;
        bool startsWithSpecialPrefix;
        std::optional<ListItemInfo> listItemInfo;
        Range initialWhitespace;
        uint32_t endColumn;

        explicit LineInfo(const DocumentSlice& line);
    };

    struct TextReflower {
        std::string_view linePrefix;

        // this is in addition to the line prefix length
        uint32_t newLineIndent;

        uint32_t targetLineLength;

        std::vector<std::vector<std::string_view>> lines;

        std::vector<std::string_view> currentLine;
        uint32_t currentLineLength;
        uint32_t currentWordCount;
        uint32_t startingLineLength;

        TextReflower(uint32_t startingLineLength, uint32_t targetLineLength);

        void addWords(const DocumentSlice& line);
        void addWhitespace(const DocumentSlice& whitespace);
        void addLineAsIs(const DocumentSlice& line);
        void addEmptyLine();

        void startNewLine();
        void startNewLineIfNotEmpty();

        void setIndentation(uint32_t newIndentation);

        void startNewListItem(const ListItemInfo& info);
        void endListItem();

        void startNewParagraph();
    };

}
