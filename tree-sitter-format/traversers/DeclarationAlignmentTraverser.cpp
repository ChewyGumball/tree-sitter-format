#include <tree-sitter-format/traversers/DeclarationAlignmentTraverser.h>

#include <tree-sitter-format/Constants.h>
#include <tree-sitter-format/Util.h>

#include <assert.h>
#include <unordered_map>

namespace {
    using namespace tree_sitter_format;

    void AlignNodes(const std::vector<TSNode>& nodes, TraverserContext& context) {
        uint32_t maxColumn = 0;

        for(TSNode node : nodes) {
            maxColumn = std::max(maxColumn, ts_node_start_point(node).column);
        }

        for(TSNode node : nodes) {
            uint32_t startColumn = ts_node_start_point(node).column;

            assert(startColumn <= maxColumn);
            if (startColumn == maxColumn) {
                continue;
            }

            uint32_t spaceToAdd = maxColumn - startColumn;

            context.edits.push_back(InsertEdit{.position = Position::StartOf(node), .bytes = GetSpaces(spaceToAdd)});
        }
    }

    void AlignDeclarations(TSNode node, const std::vector<uint32_t>& children, TraverserContext& context) {
        std::vector<TSNode> identifiers;

        for(uint32_t i : children) {
            TSNode child = ts_node_child(node, i);
            assert(IsDeclarationLike(child));

            TSNode firstDeclarator = ts_node_child_by_field_name(child, "declarator", 10);
            assert(!ts_node_is_null(firstDeclarator));

            while(!IsIdentifierLike(firstDeclarator)) {
                if (ts_node_symbol(firstDeclarator) == INIT_DECLARATOR) {
                    firstDeclarator = ts_node_child(firstDeclarator, 0);
                }

                if (ts_node_symbol(firstDeclarator) == ATTRIBUTED_DECLARATOR) {
                    firstDeclarator = ts_node_child(firstDeclarator, 0);
                }

                if (ts_node_symbol(firstDeclarator) == POINTER_DECLARATOR) {
                    firstDeclarator = ts_node_child_by_field_name(firstDeclarator, "declarator", 10);
                }

                if (ts_node_symbol(firstDeclarator) == FUNCTION_DECLARATOR) {
                    firstDeclarator = ts_node_child_by_field_name(firstDeclarator, "declarator", 10);
                }

                if (ts_node_symbol(firstDeclarator) == ARRAY_DECLARATOR) {
                    firstDeclarator = ts_node_child_by_field_name(firstDeclarator, "declarator", 10);
                }

                if (ts_node_symbol(firstDeclarator) == PARENTHESIZED_DECLARATOR) {
                    firstDeclarator = FindFirstNonExtraChild(firstDeclarator, 1);
                    assert(!ts_node_is_null(firstDeclarator));
                }

                if (ts_node_symbol(firstDeclarator) == FIELD_DECLARATOR) {
                    firstDeclarator = ts_node_child_by_field_name(firstDeclarator, "declarator", 10);
                }
            }

            identifiers.push_back(firstDeclarator);
        }

        AlignNodes(identifiers, context);
    }

    void CheckVariables(TSNode node, const Style::Alignment& style, TraverserContext& context) {
        uint32_t childCount = ts_node_child_count(node);

        std::vector<std::vector<uint32_t>> consecutiveDeclarations;

        for(uint32_t i = 0; i < childCount; i++) {
            TSNode child = ts_node_child(node, i);

            // If this declaration is unformattable, skip it
            if (context.document.isWithinAnUnformattableRange(Range::Of(child))) {
                continue;
            }

            if (IsDeclarationLike(child)) {
                std::vector<uint32_t> list;
                list.push_back(i++);

                uint32_t previousLine = ts_node_end_point(child).row;
                for(; i < childCount; i++) {
                    TSNode c = ts_node_child(node, i);

                    // If this declaration is unformattable, break the chain of aligned declarations.
                    // It will be skipped in the next iteration of the outer loop.
                    if (context.document.isWithinAnUnformattableRange(Range::Of(c))) {
                        break;
                    }

                    uint32_t currentLine = ts_node_end_point(c).row;

                    bool isSameLine = previousLine + 1 != currentLine;
                    previousLine = currentLine;

                    if (style.acrossComments && ts_node_is_extra(c)) {
                        continue;
                    }

                    if (IsDeclarationLike(c)) {
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
            AlignDeclarations(node, list, context);
        }
    }
}

namespace tree_sitter_format {
void DeclarationAlignmentTraverser::postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    // We need to look at all children all at once, not in a depth first fashion. We do that when we get called
    // for the first child, and do nothing for the other children. We can't look at the child because that would
    // miss the top level node which can have declarations in it.
    if (childIndex != 0) {
        return;
    }

    TSSymbol symbol = ts_node_symbol(node);

    if (context.style.alignment.variableDeclarations.align) {
        if (symbol == TRANSLATION_UNIT || symbol == COMPOUND_STATEMENT) {
            CheckVariables(node, context.style.alignment.variableDeclarations, context);
        }
    }

    if (context.style.alignment.memberVariableDeclarations.align) {
        if (symbol == FIELD_DECLARATION_LIST) {
            CheckVariables(node, context.style.alignment.memberVariableDeclarations, context);
        }
    }
}
}
