#include <tree_sitter_format/traversers/AlignmentTraverser.h>

#include <tree_sitter_format/Constants.h>

#include <assert.h>
#include <unordered_map>

namespace {
    using namespace tree_sitter_format;

    std::unordered_map<uint32_t, std::string> spaces;

    void AlignDeclarations(TSNode node, const std::vector<uint32_t>& children, TraverserContext& context) {
        uint32_t maxDistanceToName = 0;

        std::vector<TSNode> identifiers;

        for(uint32_t i : children) {
            TSNode child = ts_node_child(node, i);
            assert(ts_node_symbol(child) == DECLARATION);

            TSNode firstDeclarator = ts_node_child(child, 1);

            while(ts_node_symbol(firstDeclarator) != IDENTIFIER) {
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
                    firstDeclarator = ts_node_child(firstDeclarator, 1);
                }
            }

            maxDistanceToName = std::max(maxDistanceToName, ts_node_start_point(firstDeclarator).column);
            identifiers.push_back(firstDeclarator);
        }

        for(TSNode identifier : identifiers) {
            uint32_t startColumn = ts_node_start_point(identifier).column;

            assert(startColumn <= maxDistanceToName);
            if (startColumn == maxDistanceToName) {
                continue;
            }

            uint32_t spaceToAdd = maxDistanceToName - startColumn;
            if (!spaces.contains(spaceToAdd)) {
                spaces[spaceToAdd] = std::string(spaceToAdd, ' ');
            }

            context.edits.push_back(InsertEdit{.position = Position::StartOf(identifier), .bytes = spaces[spaceToAdd]});
        }
    }

    void CheckVariables(TSNode node, const Style::Alignment&, TraverserContext& context) {
        uint32_t childCount = ts_node_child_count(node);

        std::vector<std::vector<uint32_t>> consecutiveDeclarations;

        for(uint32_t i = 0; i < childCount; i++) {
            TSNode child = ts_node_child(node, i);
            TSSymbol symbol = ts_node_symbol(child);

            if (symbol == DECLARATION) {
                std::vector<uint32_t> list;

                for(; i < childCount; i++) {
                    TSNode c = ts_node_child(node, i);
                    if (ts_node_symbol(c) == DECLARATION) {
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
}
}
