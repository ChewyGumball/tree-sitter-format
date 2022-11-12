#include <tree-sitter-format/traversers/BitfieldAlignmentTraverser.h>

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
        std::vector<TSNode> bitfields;

        for(uint32_t i : children) {
            TSNode child = ts_node_child(node, i);
            assert(IsBitfieldDeclaration(child));

            uint32_t childCount = ts_node_child_count(child);
            uint32_t bitFieldIndex = childCount - 2; // -1 is the last child, bit fields are the second last child, hence -2

            TSNode firstDeclarator = FindLastNonExtraChild(child, bitFieldIndex);
            assert(!ts_node_is_null(firstDeclarator));

            bitfields.push_back(firstDeclarator);
        }

        AlignNodes(bitfields, context);
    }

    void CheckBitFields(TSNode node, const Style::Alignment& style, TraverserContext& context) {
        uint32_t childCount = ts_node_child_count(node);

        std::vector<std::vector<uint32_t>> consecutiveDeclarations;

        for(uint32_t i = 0; i < childCount; i++) {
            TSNode child = ts_node_child(node, i);

            // If this field is unformattable, skip it
            if (context.document.isWithinAnUnformattableRange(Range::Of(child))) {
                continue;
            }

            if (IsBitfieldDeclaration(child)) {
                std::vector<uint32_t> list;
                list.push_back(i++);

                uint32_t previousLine = ts_node_end_point(child).row;
                for(; i < childCount; i++) {
                    TSNode c = ts_node_child(node, i);

                    // If this field is unformattable, break the chain of aligned fields.
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

                    if (IsBitfieldDeclaration(c)) {
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
void BitfieldAlignmentTraverser::postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    if (!context.style.alignment.bitFields.align) {
        return;
    }

    TSNode child = ts_node_child(node, childIndex);

    if (ts_node_symbol(child) == FIELD_DECLARATION_LIST) {
        CheckBitFields(child, context.style.alignment.bitFields, context);
    }
}
}
