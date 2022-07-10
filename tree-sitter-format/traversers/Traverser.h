#pragma once

#include <tree_sitter/api.h>

#include <concepts>
#include <type_traits>

namespace tree_sitter_format {

template<typename TRAVERSER_TYPE, typename CONTEXT_TYPE>
concept Traverser = requires(TRAVERSER_TYPE t, CONTEXT_TYPE& c, TSNode n, uint32_t i) {
    { t.visitLeaf(n, c) } -> std::same_as<void>;
    { t.preVisitChild(n, i, c) } -> std::same_as<void>;
    { t.postVisitChild(n, i, c) } -> std::same_as<void>;
};

template<typename CONTEXT_TYPE, Traverser<CONTEXT_TYPE> TRAVERSER_TYPE>
void Traverse(TSNode node, CONTEXT_TYPE& context, TRAVERSER_TYPE& traverser) {
    if (ts_node_is_null(node)) {
        return;
    }

    uint32_t childCount = ts_node_child_count(node);
    if (childCount == 0) {
        traverser.visitLeaf(node, context);
    } else {
        for(uint32_t i = 0; i < childCount; i++) {
            traverser.preVisitChild(node, i, context);
            Traverse(ts_node_child(node, i), context, traverser);
            traverser.postVisitChild(node, i, context);
        }
    }
}

}
