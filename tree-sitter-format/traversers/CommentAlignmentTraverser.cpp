#include <tree-sitter-format/traversers/CommentAlignmentTraverser.h>

#include <tree-sitter-format/Constants.h>
#include <tree-sitter-format/Util.h>

#include <assert.h>
#include <unordered_map>

namespace {
    using namespace tree_sitter_format;

    void LeftJustifyNodes(const std::vector<TSNode>& comments, TraverserContext& context) {
        for(TSNode comment : comments) {
            // Don't change comments that are in an unformattable range
            if (context.document.overlapsUnformattableRange(Range::Of(comment))) {
                continue;
            }

            TSNode previous = FindPreviousNode(comment);

            Position previousPosition = Position::EndOf(previous);
            Position currentPosition = Position::StartOf(comment);

            if (previousPosition.location.row == currentPosition.location.row) {
                // If we are already one space after the end of the previous node on the line, we don't
                // need to do anything.
                if (previousPosition.location.column != currentPosition.location.column - 1) {
                    continue;
                }

                // Otherwise, remove all space between the previous node and the current node
                context.edits.push_back(DeleteEdit {.range = Range::Between(previousPosition, currentPosition)});
                // And insert a single space so that the trailing comment is "left justified" against the end of the line
                context.edits.push_back(InsertEdit {.position = previousPosition, .bytes = GetSpaces(1)});
            } else if (currentPosition.location.column != 0) {
                // If we are the only thing on the line, remove all preceeding whitespace
                Range preceedingWhitespace = context.document.toPreviousNewLine(currentPosition);
                context.edits.push_back(DeleteEdit {.range = preceedingWhitespace});
            }
        }
    }

    bool IsFirstNodeOnLine(TSNode node) {
        TSNode previous = FindPreviousNode(node);
        // If the previous node ends on a different line than the current node starts,
        // the current node is the first node on the line.
        return ts_node_end_point(previous).row != ts_node_start_point(node).row;
    }

    Range ToEndOfPreviousNode(TSNode node) {
        TSNode previous = FindPreviousNode(node);
        return Range::Between(Position::EndOf(previous), Position::StartOf(node));
    }

    void AlignNodes(const std::vector<TSNode>& nodes, TraverserContext& context) {
        std::vector<Range> ranges;
        uint32_t maxColumn = 0;

        for(const TSNode& node : nodes) {
            Range toPrevious = ToEndOfPreviousNode(node);
            ranges.push_back(toPrevious);

            // this is +1 because we want the comments to be aligned one column after the end of the farthest right token
            maxColumn = std::max(maxColumn, toPrevious.start.location.column + 1);
        }

        for(size_t i = 0; i < nodes.size(); i++) {
            Range& range = ranges[i];
            context.edits.push_back(DeleteEdit{.range = range});

            uint32_t columnsToAdd = maxColumn - range.start.location.column;
            context.edits.push_back(InsertEdit{.position = ranges[i].start.byteOffset, .bytes = GetSpaces(columnsToAdd)});
        }
    }

    void AlignConsecutiveNodes(const std::vector<TSNode>& comments, TraverserContext& context) {
        // Separate comment nodes into groups where all comments in a group are on consecutive rows
        std::vector<std::vector<TSNode>> groupedComments = GroupNodesOnConsecutiveLines(comments);

        // Align the starts of the comments in each group
        for(auto& group : groupedComments) {
            AlignNodes(group, context);
        }
    }
}

namespace tree_sitter_format {
void CommentAlignmentTraverser::reset(const TraverserContext&) {
    commentNodes.clear();
}

void CommentAlignmentTraverser::visitLeaf(TSNode node, TraverserContext& context) {
    if (context.style.alignment.trailingComments == Style::TrailingCommentAlignment::Ignore) {
        return;
    }

    // We only collect comment nodes here, because trailing comments are not always
    // siblings, so it is easier to collect them all, then align them at the end.
    if (ts_node_symbol(node) == COMMENT) {

        // Ignore any comments in unformattable ranges
        if (context.document.overlapsUnformattableRange(Range::Of(node))) {
            return;
        }

        // We only care about trailing comments. Comments on a line by themselves will be
        // aligned based on scoping rules.
        if (IsFirstNodeOnLine(node)) {
            return;
        }

        commentNodes.push_back(node);
    }
}

void CommentAlignmentTraverser::postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    // We want to process all the comments after looking at all the nodes. That means after the last child of the
    // translation unit.

    // If we aren't the translation unit, nothing to do
    if (ts_node_symbol(node) != TRANSLATION_UNIT) {
        return;
    }

    // If the translation unit is empty, or we didn't just finish looking at the last child,
    // nothing to do
    uint32_t childCount = ts_node_child_count(node);
    if (childCount == 0 || childIndex != childCount - 1) {
        return;
    }

    if (context.style.alignment.trailingComments == Style::TrailingCommentAlignment::LeftJustify) {
        LeftJustifyNodes(commentNodes, context);
    } else if (context.style.alignment.trailingComments == Style::TrailingCommentAlignment::AlignConsecutive) {
        AlignConsecutiveNodes(commentNodes, context);
    }
}
}
