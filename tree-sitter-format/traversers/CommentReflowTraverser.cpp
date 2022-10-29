#include <tree_sitter_format/traversers/CommentReflowTraverser.h>

#include <tree_sitter_format/Constants.h>
#include <tree_sitter_format/Util.h>
#include <tree_sitter_format/document/Document.h>

#include <assert.h>
#include <unordered_map>
#include <forward_list>
#include <iostream>

// At some point, look at https://github.com/CLD2Owners/cld2 to detect locale
// then use https://unicode-org.github.io/icu/userguide/boundaryanalysis/ to 
// do word and line break detection not based on spaces and CL/LF characters.

using namespace std::literals::string_view_literals;

namespace {
    using namespace tree_sitter_format;

    [[nodiscard]] bool IsMultiLineComment(TSNode node, const Document& document) {
        if (ts_node_symbol(node) != COMMENT) {
            return false;
        }

        std::string_view text = document.originalContentsAt(Range::Of(node));
        return text.starts_with("/*");
    }

    [[nodiscard]] bool IsSingleLineComment(TSNode node, const Document& document) {
        if (ts_node_symbol(node) != COMMENT) {
            return false;
        }

        std::string_view text = document.originalContentsAt(Range::Of(node));
        return text.starts_with("//");        
    }

    // void ReflowComments(TSNode node, const std::vector<uint32_t>& children, TraverserContext& context) {
    //     // 3. consecutive single line comments will be merged if they start at the same column and have no
    //     //      empty lines between them
    // }

    bool IsWhitespace(char32_t character) {
        return character == ' ' || character == '\t';
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

    std::vector<std::vector<Word>> SeparateWords(const std::vector<DocumentSlice>& lines) {
        std::vector<std::vector<Word>> words;
        for(const DocumentSlice& line : lines) {
            words.emplace_back(SeparateWords(line));
        }

        return words;
    }

    std::vector<std::vector<std::string_view>> ReflowParagraph(const std::vector<std::vector<Word>>& lines, size_t startLine, size_t endLine, uint32_t firstLineOffset, uint32_t targetLineLength) {
        std::vector<std::vector<std::string_view>> reflowedLines;

        uint32_t currentLineLength = firstLineOffset;
        std::vector<std::string_view> currentLine;

        for(size_t lineIndex = startLine; lineIndex < endLine; lineIndex++) {
            for(const Word& word : lines[lineIndex]) {
                currentLineLength += word.length;

                // If adding the next word would cause the current line to go past the target line length,
                // record the line, then start a new line. Also, we want at least one word per line, so
                // if the current line is empty, don't start a new line yet.
                if (currentLineLength > targetLineLength && !currentLine.empty()) {
                    // The last element is a space, which we don't need to keep so remove it before
                    // recording the line.
                    currentLine.pop_back();

                    // Record the current line
                    reflowedLines.push_back(currentLine);

                    // Start a new line
                    currentLine.clear();
                    currentLineLength = word.length;
                }

                // Add the next word to the line
                currentLine.insert(currentLine.end(), word.elements.begin(), word.elements.end());

                // Add a space to the line
                currentLine.push_back(" "sv);
                currentLineLength++;
            }       
        }

        // If we had something on the last line, record it
        if (!currentLine.empty()) {
            currentLine.pop_back();
            reflowedLines.push_back(currentLine);
        }

        return reflowedLines;
    }

    std::vector<std::vector<std::string_view>> ReflowLines(const std::vector<DocumentSlice> & lines, uint32_t firstLineOffset, uint32_t nonFirstLineOffset, uint32_t targetLineLength) {
        std::vector<std::vector<Word>> words = SeparateWords(lines);

        size_t startLine = 0;
        size_t endLine = 0;

        std::vector<std::vector<std::string_view>> reflowedLines;
        for(const std::vector<Word>& line : words) {
            if (line.empty()) {

                // This can happen if there are multiple blank lines in a row
                if (startLine != endLine) {
                    // Reflow the paragraph up to this blank line
                    const bool isFirstLine = startLine == 0;
                    uint32_t offset = isFirstLine ? firstLineOffset : nonFirstLineOffset;
                    std::vector<std::vector<std::string_view>> reflowed = ReflowParagraph(words, startLine, endLine, offset, targetLineLength);

                    // Record the paragraph
                    reflowedLines.insert(reflowedLines.end(), reflowed.begin(), reflowed.end());
                }

                // Add this blank line
                reflowedLines.emplace_back();

                startLine = endLine + 1;
            }

            endLine++;
        }

        // Add the last unrecorded paragraph if there is one
        if (startLine != endLine) {
            const bool isFirstLine = startLine == 0;
            uint32_t offset = isFirstLine ? firstLineOffset : nonFirstLineOffset;
            std::vector<std::vector<std::string_view>> reflowed = ReflowParagraph(words, startLine, endLine, offset, targetLineLength);
            reflowedLines.insert(reflowedLines.end(), reflowed.begin(), reflowed.end());
        }

        return reflowedLines;
    }

    bool RemovePrefix(std::vector<DocumentSlice>& lines, const std::string_view& prefix, const std::string_view& lastLinePrefix) {
        bool allSame = true;

        // We skip the first line because it starts with a /* always
        for(size_t i = 1; allSame && i < lines.size(); i++) {
            const std::string_view& check = i == lines.size() - 1 ? lastLinePrefix : prefix;
            if (!lines[i].startsWith(check)) {
                allSame = false;
            }
        }

        if (allSame) {
            for(size_t i = 1; allSame && i < lines.size(); i++) {
                const std::string_view& remove = i == lines.size() - 1 ? lastLinePrefix : prefix;

                Position newStart = lines[i].startPosition();
                newStart.location.column += uint32_t(remove.size());
                newStart.byteOffset += uint32_t(remove.size());
                
                Range trimmedRange {
                    .start = newStart,
                    .end = lines[i].endPosition(),
                };

                lines[i] = lines[i].slice(trimmedRange);
            }
        }

        return allSame;
    }

    void ReflowMultiLineComment(TSNode node, TraverserContext& context) {
        // 1. multiline comments where all lines start with " *" will continue to do so after reflow.
        // 2. consecutive multiline comments will not be merged
        // 3. empty lines at the beginning of a multiline comment will be removed.
        // 4. if a word on a line by itself is still past the line length, the line length is ignored
        //       and the word is allowed on that line (it is a "target" line length after all).
        
        assert(IsMultiLineComment(node, context.document));

        std::vector<DocumentSlice> lines;

        // find the start not including the /*
        Position start = Position::StartOf(node);
        start.location.column += 2;
        start.byteOffset += 2;

        // find the end not including the */
        Position end = Position::EndOf(node);
        end.location.column -= 2;
        end.byteOffset -= 2;

        while (start != end) {
            Range nextNewLine = context.document.nextNewLine(start);

            if (nextNewLine.start.byteOffset > end.byteOffset) {
                lines.push_back(context.document.slice(Range::Between(start, end)));
                start = end;
            } else {
                lines.push_back(context.document.slice(Range::Between(start, nextNewLine.start)));
                start = nextNewLine.end;
            }
        }

        lines.push_back(context.document.slice(Range::Between(start, end)));

        std::string_view prefix = " *"sv;
        bool allSame = RemovePrefix(lines, prefix, " "sv);

        // If the lines didn't already start with a prefix, we will start them with
        // some blank spaces to ensure alignment.
        if (!allSame) {
            prefix = "  "sv;
        }

        // We want non first line offset to be one more than the prefix because we will add a space between the prefix and the first word
        // on the line.
        uint32_t firstLineOffset = start.location.column + 1;
        uint32_t nonFirstLineOffset = uint32_t(prefix.size() + 1);
        std::vector<std::vector<std::string_view>> reflowedLines = ReflowLines(lines, firstLineOffset, nonFirstLineOffset, context.style.targetLineLength);
        
        // Delete the whole comment, we will recreate it from the reflowed lines
        context.edits.emplace_back(DeleteEdit{.range = Range::Of(node)});

        // We have to add it back in backwards, because edits are applied from greatest to least byte offset. If we did it forward,
        // our inserts would be applied before the delete, but then the delete would delete them :(
        
        Position insertPoint = Position::StartOf(node);

        auto insert = [&](const std::string_view& element) {
            context.edits.emplace_back(InsertEdit{.position = insertPoint, .bytes = element});
        };

        auto insertLine = [&](const std::vector<std::string_view>& line) {
            for(auto elementIndex = std::ssize(line) - 1; elementIndex >= 0; elementIndex--) {
                insert(line[elementIndex]);
            }
        };

        insert("*/"sv);

        // If the commend ends on a line with text, we want to add a space between the text and the commend end
        if (!reflowedLines.back().empty()) {
            insert(" "sv);
        }

        for(auto lineIndex = std::ssize(reflowedLines) - 1; lineIndex > 0; lineIndex--) {
            insertLine(reflowedLines[lineIndex]);
            insert(" "sv);
            insert(prefix);
            insert(context.style.newLineString());
        }

        insertLine(reflowedLines[0]);
        insert("/* "sv);
    }
}

namespace tree_sitter_format {
void CommentReflowTraverser::reset(const TraverserContext&) {}

void CommentReflowTraverser::visitLeaf(TSNode, TraverserContext&) {}
void CommentReflowTraverser::preVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    // We need to look at all children all at once, not in a depth first fashion. We do that when we get called
    // for the first child, and do nothing for the other children. We can't look at the child because we want
    // to be able to align consecutive single line comments together.
    if (childIndex != 0) {
        return;
    }

    if (!context.style.comments.reflow) {
        return;
    }

    uint32_t childCount = ts_node_child_count(node);
    for(uint32_t i = 0; i < childCount; i++) {
        TSNode child = ts_node_child(node, i);
        if (IsMultiLineComment(child, context.document)) {
            ReflowMultiLineComment(child, context);
        }
    }


    std::vector<std::vector<uint32_t>> consecutiveComments;

    for(uint32_t i = 0; i < childCount; i++) {
        TSNode child = ts_node_child(node, i);

        if (IsSingleLineComment(child, context.document)) {
            std::vector<uint32_t> list;
            list.push_back(i++);

            uint32_t startColumn = ts_node_start_point(child).column;
            uint32_t previousLine = ts_node_end_point(child).row;
            for(; i < childCount; i++) {
                TSNode c = ts_node_child(node, i);

                uint32_t currentStartColumn = ts_node_start_point(c).column;
                uint32_t currentLine = ts_node_end_point(c).row;

                bool isSameLine = previousLine + 1 != currentLine;
                bool isSameColumn = currentStartColumn == startColumn;
                
                if (!isSameLine || !isSameColumn || !IsSingleLineComment(c, context.document)) {
                    break;
                }
                
                list.push_back(i);
            }

            if (list.size() > 1) {
                consecutiveComments.push_back(std::move(list));
            }

            // We have to backtrack one beacuse node we looked at last may still be a single line comment
            // it just might not have started on the same column, or maybe there were blank lines between.
            i--;
        }
    }

    // for(const auto& list : consecutiveComments) {
    //     ReflowComments(node, list, context);
    // }
}
void CommentReflowTraverser::postVisitChild(TSNode, uint32_t, TraverserContext&) {}
}
