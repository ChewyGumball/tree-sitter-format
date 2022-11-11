#include <tree-sitter-format/traversers/CommentReflowTraverser.h>

#include <tree-sitter-format/Constants.h>
#include <tree-sitter-format/Util.h>
#include <tree-sitter-format/document/Document.h>
#include <tree-sitter-format/document/TextReflower.h>

#include <array>
#include <assert.h>
#include <unordered_map>
#include <forward_list>
#include <iostream>
#include <variant>
#include <vector>

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
    
    void RemovePrefix(std::vector<DocumentSlice>& lines, const std::string_view& prefix) {
        bool allSame = lines.size() > 1;
        size_t lastLineToCheck = lines.size();

        // If the last line only has the close comment token on it, don't check it for a prefix since
        // it doesn't need one. The close token has already been removed, so if we trim the last line
        // and it is empty, then it only had the close comment token.
        if (lastLineToCheck > 1) {
            std::string lastLine = lines.back().trimFront().toString();

            if (lastLine == "") {
                lastLineToCheck--;
            }
        }

        // We skip the first line because it starts with  "/*" always
        for(size_t i = 1; allSame && i < lastLineToCheck; i++) {
            if (!lines[i].startsWith(prefix)) {
                allSame = false;
            }
        }

        // If all the lines start with the same prefix, remove the prefix!
        if (allSame) {
            std::vector<std::string_view> prefixVector = {prefix};
            UnicodeIterator prefixIterator(prefixVector);
            while(!prefixIterator.atEnd()) {
                prefixIterator++;
            }

            for(size_t i = 1; allSame && i < lastLineToCheck; i++) {
                Position newStart = lines[i].startPosition();
                newStart.location.column += prefixIterator.currentPosition().location.column;
                newStart.byteOffset += prefixIterator.currentPosition().byteOffset;
                
                Range trimmedRange {
                    .start = newStart,
                    .end = lines[i].endPosition(),
                };

                lines[i] = lines[i].slice(trimmedRange);
            }
        }
    }

    [[nodiscard]] bool LineIsEmpty(const DocumentSlice& line) {
        return line.trimFront().range().byteCount() == 0;
    }

    void RemoveExtraBlankLines(std::vector<DocumentSlice>& lines) {
        for(size_t i = 0; i < lines.size(); i++) {
            if (LineIsEmpty(lines[i])) {
                while(i + 1 < lines.size() && LineIsEmpty(lines[i+1])) {
                    lines.erase(lines.begin() + i + 1);
                }
            }
        }
    }
    
    [[nodiscard]] std::vector<DocumentSlice> BreakIntoLines(TSNode node, TraverserContext& context) {
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

        std::string_view prefix = " * "sv;
        RemovePrefix(lines, prefix);
        RemoveExtraBlankLines(lines);

        return lines;
    }

    [[nodiscard]] bool CanReflow(const LineInfo& lineInfo) {
        return !lineInfo.startsWithPunctuation && !lineInfo.isEmpty;
    }

    [[nodiscard]] bool LineWasOriginallyTooLong(const LineInfo& lineInfo, TraverserContext& context) {
        return lineInfo.endColumn > context.style.targetLineLength;
    }

    [[nodiscard]] bool CanBeReflowedOnto(const LineInfo& lineInfo) {
        // If the line starts with more than two spaces or a tab, it won't be reflowed onto
        if (lineInfo.startsWithWhitespace) {
            return false;
        }

        // Empty lines won't be reflowed onto
        if (lineInfo.isEmpty) {
            return false;
        }

        // List items, or lines that start with multiple punctuation characters can't be reflowed onto
        if (lineInfo.listItemInfo.has_value() || lineInfo.startsWithPunctuation) {
            return false;
        }

        // If the line starts with any special prefixes, it won't be reflowed onto.
        if (lineInfo.startsWithSpecialPrefix) {
            return false;
        }

        return true;
    }

    [[nodiscard]] bool MatchesListItemIndentation(const LineInfo& lineInfo, const DocumentSlice& number) {
        // If this line starts a line item, it doesn't match previous line items
        if (lineInfo.listItemInfo.has_value()) {
            return false;
        }

        uint32_t startColumn = number.startPosition().location.column;
        uint32_t endColumn = number.endPosition().location.column + 1;
        uint32_t firstNonWhitespaceColumn = lineInfo.initialWhitespace.end.location.column;

        return firstNonWhitespaceColumn >= startColumn && firstNonWhitespaceColumn <= endColumn;
    }

    std::vector<std::vector<std::string_view>> ReflowLines(std::vector<DocumentSlice>& lines, uint32_t firstLineOffset, TraverserContext& context) {
        TextReflower comment(firstLineOffset, context.style.targetLineLength);
        std::optional<ListItemInfo> currentListItem;

        for(const DocumentSlice& line : lines) {
            LineInfo info(line);

            // If this line starts a list item
            if (info.listItemInfo.has_value()) {
                currentListItem = info.listItemInfo;

                // Start a new list item in the comment
                comment.startNewListItem(info.listItemInfo.value());

                Range rangeWithoutNumber = Range::Between(info.listItemInfo->number.endPosition(), line.endPosition());
                DocumentSlice lineWithoutNumber = line.slice(rangeWithoutNumber);
                comment.addWords(lineWithoutNumber);

                continue;
            }

            if (currentListItem.has_value() && (CanBeReflowedOnto(info) || MatchesListItemIndentation(info, currentListItem->number))) {
                comment.addWords(line);
                continue;
            } else if (currentListItem.has_value()) {
                currentListItem = std::nullopt;
                comment.endListItem();
            }

            if (info.isEmpty) {
                comment.addEmptyLine();
                continue;
            }

            // If we can't be reflowed onto, and the current line isn't empty, we want to start a new line
            if (!CanBeReflowedOnto(info)) {
                comment.startNewLineIfNotEmpty();
            }

            if (CanReflow(info)) {
                if (info.startsWithWhitespace) {
                    comment.addWhitespace(line.slice(info.initialWhitespace));
                }
                // If we can reflow this line, we want to separate it into words first
                comment.addWords(line);
            } else {
                // Otherwise just add it as is
                comment.addLineAsIs(line);
            }

            if (!LineWasOriginallyTooLong(info, context)) {
                comment.startNewLineIfNotEmpty();
            }
        }

        // Always end on a fresh line so the end comment token is by itself
        comment.startNewLineIfNotEmpty();

        return comment.lines;
    }

    void ReflowMultiLineComment(TSNode node, TraverserContext& context) {
        // 1. All lines other than the first will have the prefix " * " added to it. If the last line
        // only contains "*/" or " */", no prefix will be added to it. If all lines other than the
        // first, and last if it only contains the end comment token, already start with " * ", no
        // prefix will be added, and the prefix will not be considered for the following rules.
        //
        // 2. Empty lines can not be reflowed onto. This means if the previous line needs to reflow
        // because it is too long, it will create a new line to reflow onto rather than fill up the
        // empty line.
        //
        // 3. Lines that start with more than 2 spaces, or 1 tab, of white space will not be reflowed
        // onto, but they may be reflowed. The start of the line is probably formatted intentionally
        // by the author.
        //
        // 4. Lines that start with 2 or more punctuation characters will not be reflowed onto, nor
        // reflowed. This line is probably formatted intentionally by the author to create a graphic,
        // or table, or something.
        //
        // 5. Lines that start with one of the following strings will not be reflowed onto, as they
        // are probably intended to be separated from previous text: "TODO", "FIXME", "BUG", "@",
        // "* ", "- "
        //
        // 6. Lines that start with a number followed by a period will not be reflowed onto, but may
        // be reflowed. These lines are probably a list.
        //
        // The number may be a up to 2 digits, as it is unlikely a list will be more than 99 items
        // long in a comment, and we want to minimize miscategorizing lines that end a sentence with
        // a number.
        //
        // Following lines will be considered part of the list item if they can be reflowed onto, or
        // they are indented to between the start of the number that started the list item to one past
        // the period. This is to catch multiline list items that have already been indented to align
        // with the number. 
        //
        // The start of a list item will be indented by two spaces if it isn't already indented more,
        // and following lines of a list item will be indented to the column one past the period that
        // started the list item. The first word of a list item will be placed one column after the
        // period that started the list item if it is not already.
        //
        // 7. Multiple empty, or white space only lines will be condensed into a single empty line.
        //
        // 8. If a word is on a line by itself, either authored as such, or through reflowing, and the
        // word still goes past the line length, the line length will be ignored.
        //
        // 9. If a line does not go past the line length as authored, it may be reflowed if the previous
        // line reflows onto it and pushes it past the line length, but it will create a new line to
        // reflow onto, rather than reflowing onto the next authored line. If the author didn't use up
        // the whole line, it was probably by design.
        //
        // 10. If a line is reflowed, the line will be split into "words", where a word is a string of
        // characters separated by white space. The reflowed line will have a single space between each
        // word.
        //
        // 11. If a line is not reflowed, it will remain unchanged.
        
        assert(IsMultiLineComment(node, context.document));

        std::vector<DocumentSlice> lines = BreakIntoLines(node, context);
        
        // The first line starts 3 characters after the start of the comment. IE is starts after "/* " which will be
        // the enforced start of the comment after reflow.
        Position start = Position::StartOf(node);
        uint32_t firstLineOffset = start.location.column + 3;

        std::vector<std::vector<std::string_view>> reflowedLines = ReflowLines(lines, firstLineOffset, context);
        
        // Delete the whole comment, we will recreate it from the reflowed lines
        context.edits.emplace_back(DeleteEdit{.range = Range::Of(node)});

        // We have to add things in backwards, because edits are applied from greatest to least byte offset. If we did it forward,
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


        // Add the end comment token
        insert(" */"sv);

        // If the comment is more than one line, the end comment token should be on its own line.
        if (!reflowedLines.back().empty() && reflowedLines.size() > 1) {
            insert(context.style.newLineString());
        }
        
        // Add the contents of the comment
        for(auto lineIndex = std::ssize(reflowedLines) - 1; lineIndex > 0; lineIndex--) {
            insertLine(reflowedLines[lineIndex]);
            insert(context.style.newLineString());
        }
        
        insertLine(reflowedLines[0]);

        // Add the start comment token
        insert("/*"sv);
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
