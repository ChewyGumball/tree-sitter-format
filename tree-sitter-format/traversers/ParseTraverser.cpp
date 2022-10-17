#include <tree_sitter_format/traversers/ParseTraverser.h>

#include <tree_sitter_format/Util.h>

#include <iostream>
#include <format>

namespace tree_sitter_format {
    void ParseTraverser::reset([[maybe_unused]]const TraverserContext& context) {
        index = 0;
        scope = 0;
    }

    void ParseTraverser::visitLeaf(TSNode node, [[maybe_unused]]TraverserContext& context) {
        TSNode parent = ts_node_parent(node);
        TSPoint start = ts_node_start_point(node);
        TSPoint end = ts_node_end_point(node);
        
        std::cout << std::format("[{:2}, {:2}] -> [{:2}, {:2}] ", start.row, start.column, end.row, end.column);
        for(uint32_t i = 0; i < scope; i++) {
            std::cout << "    ";
        }

        std::cout << ts_node_type(node);

        std::string_view fieldName = ChildFieldName(parent, index);
        if (!fieldName.empty()) {
            std::cout << std::format(" ({})", fieldName);
        }

        std::cout << std::endl;
    }

    void ParseTraverser::preVisitChild(TSNode node, uint32_t childIndex, [[maybe_unused]]TraverserContext& context) {
        index = childIndex;

        std::string_view fieldName = ChildFieldName(node, childIndex);

        TSNode child = ts_node_child(node, childIndex);
        bool childHasChildren = ts_node_child_count(child) > 0;

        if (childIndex == 0) {
            scope++;
        }

        if (childHasChildren) {
            visitLeaf(child, context);
        }        
    }

    void ParseTraverser::postVisitChild(TSNode node, uint32_t childIndex, [[maybe_unused]]TraverserContext& context) {
        uint32_t childCount = ts_node_child_count(node);
        if(childIndex == childCount - 1) {
            scope--;
        }
    }
}
