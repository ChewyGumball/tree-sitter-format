#include <tree_sitter_format/traversers/Traverser.h>

namespace tree_sitter_format {

void Traverser::traverse(TSNode node) {
    if (ts_node_is_null(node)) {
        return;
    }

    uint32_t childCount = ts_node_child_count(node);
    if (childCount == 0) {
        visitLeaf(node);
    } else {
        for(uint32_t i = 0; i < childCount; i++) {
            preVisitChild(node, i);
            traverse(ts_node_child(node, i));
            postVisitChild(node, i);
        }
    }
}

}
