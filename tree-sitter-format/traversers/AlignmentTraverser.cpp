#include <tree_sitter_format/traversers/AlignmentTraverser.h>

#include <tree_sitter_format/Constants.h>

#include <assert.h>
#include <unordered_map>

namespace {
    using namespace tree_sitter_format;

    std::unordered_map<uint32_t, std::string> spaces;

    TSNode findNextNonExtraChild(TSNode node, uint32_t childIndex) {
        TSNode child = ts_node_child(node, childIndex++);
        while(ts_node_is_extra(child)) {
            child = ts_node_child(node, childIndex++);
        }

        return child;
    }

    bool isDeclarationLike(TSNode node) {
        TSSymbol symbol = ts_node_symbol(node);
        return symbol == DECLARATION ||
               symbol == FIELD_DECLARATION;
    }

    bool isIdentifierLike(TSNode node) {
        TSSymbol symbol = ts_node_symbol(node);
        return symbol == IDENTIFIER ||
               symbol == FIELD_IDENTIFIER;
    }

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
            if (!spaces.contains(spaceToAdd)) {
                spaces[spaceToAdd] = std::string(spaceToAdd, ' ');
            }

            context.edits.push_back(InsertEdit{.position = Position::StartOf(node), .bytes = spaces[spaceToAdd]});
        }
    }

    void AlignDeclarations(TSNode node, const std::vector<uint32_t>& children, TraverserContext& context) {
        std::vector<TSNode> identifiers;

        for(uint32_t i : children) {
            TSNode child = ts_node_child(node, i);
            assert(isDeclarationLike(child));

            TSNode firstDeclarator = findNextNonExtraChild(child, 1);

            while(!isIdentifierLike(firstDeclarator)) {
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
                    firstDeclarator = findNextNonExtraChild(firstDeclarator, 1);
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

            if (isDeclarationLike(child)) {
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

                    if (isDeclarationLike(c)) {
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
void AlignmentTraverser::reset(const TraverserContext&) {}

void AlignmentTraverser::visitLeaf(TSNode, TraverserContext&) {}
void AlignmentTraverser::preVisitChild(TSNode, uint32_t, TraverserContext&) {}
void AlignmentTraverser::postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    // We need to look at all children all at once, not in a depth first fashion. We do that when we get called
    // for the first child, and do nothing for the other children.
    if (childIndex != 0) {
        return;
    }

    TSSymbol symbol = ts_node_symbol(node);

    if (symbol == TRANSLATION_UNIT) {
        CheckVariables(node, context.style.alignment.variableDeclarations, context);
        // CheckBitFields(node, context.style.alignment.bitFields);
        // CheckVariables(node, context.style.alignment.variableDeclarations);
        // CheckVariables(node, context.style.alignment.variableDeclarations);
    }

    if (symbol == FIELD_DECLARATION_LIST) {
        CheckVariables(node, context.style.alignment.memberVariableDeclarations, context);
    }
}
}
