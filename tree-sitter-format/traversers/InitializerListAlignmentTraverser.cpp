#include <tree_sitter_format/traversers/InitializerListAlignmentTraverser.h>

#include <tree_sitter_format/Constants.h>
#include <tree_sitter_format/Util.h>

#include <assert.h>
#include <unordered_map>
#include <iostream>

namespace {
    using namespace tree_sitter_format;

    struct InitializerListElements {
        std::vector<std::vector<TSNode>> elements;
        uint32_t spaceAdded;

        InitializerListElements(TSNode list, bool separateCommas) : spaceAdded(0) {
            uint32_t childCount = ts_node_child_count(list);

            std::vector<TSNode> element;
            for(uint32_t i = 1; i < childCount; i++) {
                TSNode child = ts_node_child(list, i);
                TSSymbol symbol = ts_node_symbol(child);

                if (!separateCommas && symbol == COMMA) {
                    element.push_back(child);
                }

                if (symbol == COMMA || symbol == RIGHT_BRACKET) {
                    elements.push_back(element);
                    element.clear();
                }
                else {
                    element.push_back(child);
                }
            }
        }
    };

    bool IsAllOnOneLine(TSNode node) {
        TSPoint start = ts_node_start_point(node);
        TSPoint end = ts_node_end_point(node);

        return start.row == end.row;
    }

    void AlignNodes(std::vector<InitializerListElements>& nodes, size_t index, const Style::Justify& justification, TraverserContext& context) {
        uint32_t maxColumn = 0;
        
        // We look at the end point because of compound assignment operators.
        // The '=' character is always the last one, and thats what we want
        // to align.
        for(const InitializerListElements& e: nodes) {
            if (index >= e.elements.size() || e.elements[index].size() == 0) {
                continue;
            }

            uint32_t start = ts_node_start_point(e.elements[index].front()).column;
            uint32_t end = ts_node_end_point(e.elements[index].back()).column;
            maxColumn = std::max(maxColumn, end - start);
        }

        for(InitializerListElements& e: nodes) {
            if (index >= e.elements.size() || e.elements[index].size() == 0) {
                continue;
            }

            const TSNode& startNode = e.elements[index].front();
            const TSNode& endNode = e.elements[index].back();

            uint32_t start = ts_node_start_point(startNode).column;
            uint32_t end = ts_node_end_point(endNode).column;
            uint32_t width = end - start;

            assert(width <= maxColumn);
            if (width == maxColumn) {
                continue;
            }

            uint32_t spaceToAdd = maxColumn - width;
            e.spaceAdded += spaceToAdd;

            Position insertPosition = justification == Style::Justify::Left ? Position::EndOf(endNode) : Position::StartOf(startNode);
            context.edits.push_back(InsertEdit{.position = insertPosition, .bytes = GetSpaces(spaceToAdd)});
        }
    }

    void AlignInitializerLists(TSNode node, const std::vector<uint32_t>& children, bool separateCommas, const Style::Justify& justification, TraverserContext& context) {
        // Align list elements
        std::vector<InitializerListElements> iterators;
        size_t maxElements = 0;
        for(uint32_t childIndex : children) {
            InitializerListElements& elements = iterators.emplace_back(ts_node_child(node, childIndex), separateCommas);
            maxElements = std::max(maxElements, elements.elements.size());
        }

        for(size_t i = 0; i < maxElements; i++) {
            AlignNodes(iterators, i, justification, context);
        }

        // Align end bracket
        uint32_t newEndColumn = 0;
        for(int i = 0; i < children.size(); i++) {
            uint32_t originalEnd = ts_node_end_point(ts_node_child(node, children[i])).column;
            uint32_t spaceAdded = iterators[i].spaceAdded;

            newEndColumn = std::max(newEndColumn, originalEnd + spaceAdded);
        }

        for(int i = 0; i < children.size(); i++) {
            TSNode list = ts_node_child(node, children[i]);
            uint32_t endBracketIndex = ts_node_child_count(list) - 1;
            TSNode endBracket = ts_node_child(list, endBracketIndex);
            uint32_t originalEnd = ts_node_end_point(list).column;
            uint32_t spaceAdded = iterators[i].spaceAdded;

            uint32_t spaceToAdd = newEndColumn - (originalEnd + spaceAdded);
            context.edits.push_back(InsertEdit{.position = Position::StartOf(endBracket), .bytes = GetSpaces(spaceToAdd)});
        }
    }

    void CheckList(TSNode node, const Style::Alignment& style, bool separateCommas, const Style::Justify& justification, TraverserContext& context) {
        uint32_t childCount = ts_node_child_count(node);

        std::vector<std::vector<uint32_t>> consecutiveInitializerLists;

        // We skip every other child because that is the comma in the list
        for(uint32_t i = 1; i < childCount; i+=2) {
            TSNode child = ts_node_child(node, i);
            TSSymbol symbol = ts_node_symbol(child);
            std::cout << ts_node_type(child) << std::endl;

            if (symbol == INITIALIZER_LIST && IsAllOnOneLine(child)) {
                std::vector<uint32_t> list;
                list.push_back(i);
                i+=2;

                uint32_t previousLine = ts_node_end_point(child).row;
                for(; i < childCount; i+=2) {
                    TSNode c = ts_node_child(node, i);
                    uint32_t currentLine = ts_node_end_point(c).row;

                    bool isSameLine = previousLine + 1 != currentLine;
                    previousLine = currentLine;
                    
                    if (style.acrossComments && ts_node_is_extra(c)) {
                        continue;
                    }

                    if (ts_node_symbol(c) == INITIALIZER_LIST && IsAllOnOneLine(c)) {
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
                    consecutiveInitializerLists.push_back(std::move(list));
                }
            }
        }

        for(const auto& list : consecutiveInitializerLists) {
            AlignInitializerLists(node, list, separateCommas, justification, context);
        }
    }
}

namespace tree_sitter_format {
void InitializerListAlignmentTraverser::reset(const TraverserContext&) {}

void InitializerListAlignmentTraverser::visitLeaf(TSNode, TraverserContext&) {}
void InitializerListAlignmentTraverser::preVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    const auto& style = context.style.alignment.initializerLists;
    if (!style.alignment.align) {
        return;
    }
    TSNode child = ts_node_child(node, childIndex);
    TSSymbol symbol = ts_node_symbol(child);
    if (symbol == INITIALIZER_LIST) {
        CheckList(child, style.alignment, style.alignCommasSeparately, style.justification, context);
    }
}
void InitializerListAlignmentTraverser::postVisitChild(TSNode, uint32_t, TraverserContext&) {}
}
