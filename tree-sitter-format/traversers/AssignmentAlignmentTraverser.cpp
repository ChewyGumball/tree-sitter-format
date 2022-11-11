#include <tree-sitter-format/traversers/AssignmentAlignmentTraverser.h>

#include <tree-sitter-format/Constants.h>
#include <tree-sitter-format/Util.h>

#include <assert.h>
#include <unordered_map>

namespace {
    using namespace tree_sitter_format;

    void AlignNodes(const std::vector<TSNode>& nodes, TraverserContext& context) {
        uint32_t maxColumn = 0;
        
        // We look at the end point because of compound assignment operators.
        // The '=' character is always the last one, and thats what we want
        // to align.
        for(TSNode node : nodes) {
            maxColumn = std::max(maxColumn, ts_node_end_point(node).column);
        }

        for(TSNode node : nodes) {
            uint32_t startColumn = ts_node_end_point(node).column;

            assert(startColumn <= maxColumn);
            if (startColumn == maxColumn) {
                continue;
            }

            uint32_t spaceToAdd = maxColumn - startColumn;
            context.edits.push_back(InsertEdit{.position = Position::StartOf(node), .bytes = GetSpaces(spaceToAdd)});
        }
    }

    TSNode GetOperator(TSNode node) {
        TSSymbol symbol = ts_node_symbol(node);
        if (symbol == EXPRESSION_STATEMENT) {
            // This would be an empty statement terminated by a semicolon
            assert(ts_node_child_count(node) > 1);

            TSNode child = ts_node_child(node, 0);
            assert(ts_node_symbol(child) == ASSIGNMENT_EXPRESSION);
            return ts_node_child_by_field_name(child, "operator", 8);
        } else if (symbol == DECLARATION) {
                TSNode firstDeclarator = FindFirstNonExtraChild(node, 1);
                assert(!ts_node_is_null(firstDeclarator));
                assert(ts_node_symbol(firstDeclarator) == INIT_DECLARATOR);

                TSNode operatorNode = FindFirstNonExtraChild(firstDeclarator, 1);
                assert(!ts_node_is_null(operatorNode));

                return operatorNode;
        } else if (symbol == FIELD_DECLARATION) {
            TSNode defaultValue = ts_node_child_by_field_name(node, "default_value", 13);
            assert(!ts_node_is_null(defaultValue));

            TSNode operatorNode = ts_node_prev_sibling(defaultValue);
            while (ts_node_is_extra(operatorNode)) {
                operatorNode = ts_node_prev_sibling(operatorNode);
            }

            return operatorNode;
        }

        assert(false);
        return NullNode();
    }

    void AlignAssigments(TSNode node, const std::vector<uint32_t>& children, TraverserContext& context) {
        std::vector<TSNode> assignments;

        for(uint32_t i : children) {
            TSNode child = ts_node_child(node, i);
            assert(IsAssignmentLike(child));

            TSNode operatorNode = GetOperator(child);
            assert(!ts_node_is_null(operatorNode));

            assignments.push_back(operatorNode);
        }

        AlignNodes(assignments, context);
    }

    void CheckAssignments(TSNode node, const Style::Alignment& style, TraverserContext& context) {
        uint32_t childCount = ts_node_child_count(node);

        std::vector<std::vector<uint32_t>> consecutiveDeclarations;

        for(uint32_t i = 0; i < childCount; i++) {
            TSNode child = ts_node_child(node, i);

            if (IsAssignmentLike(child)) {
                std::vector<uint32_t> list;
                list.push_back(i++);

                uint32_t previousLine = ts_node_end_point(child).row;
                for(; i < childCount; i++) {
                    TSNode c = ts_node_child(node, i);
                    uint32_t currentLine = ts_node_end_point(c).row;

                    bool isSameLine = previousLine + 1 != currentLine;
                    previousLine = currentLine;
                    
                    if (style.acrossComments && ts_node_is_extra(c)) {
                        continue;
                    }

                    if (IsAssignmentLike(c)) {
                        if (!style.acrossEmptyLines && isSameLine) {
                            break;
                        }
                        list.push_back(i);
                    }
                    else {
                        break;
                    }
                }

                if (list.size() > 1) {
                    consecutiveDeclarations.push_back(std::move(list));
                }
            }
        }

        for(const auto& list : consecutiveDeclarations) {
            AlignAssigments(node, list, context);
        }
    }
}

namespace tree_sitter_format {
void AssignmentAlignmentTraverser::reset(const TraverserContext&) {}

void AssignmentAlignmentTraverser::visitLeaf(TSNode, TraverserContext&) {}
void AssignmentAlignmentTraverser::preVisitChild(TSNode, uint32_t, TraverserContext&) {}
void AssignmentAlignmentTraverser::postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    if (!context.style.alignment.assignments.align) {
        return;
    }

    // We need to look at all children all at once, not in a depth first fashion. We do that when we get called
    // for the first child, and do nothing for the other children. We can't look at the child because that would
    // miss the top level node which can have asignments in it.
    if (childIndex != 0) {
        return;
    }
    TSSymbol symbol = ts_node_symbol(node);

    if (symbol == TRANSLATION_UNIT || symbol == FIELD_DECLARATION_LIST || symbol == COMPOUND_STATEMENT) {
        CheckAssignments(node, context.style.alignment.assignments, context);
    }
}
}
