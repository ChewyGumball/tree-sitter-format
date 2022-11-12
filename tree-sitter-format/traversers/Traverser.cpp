#include <tree-sitter-format/traversers/Traverser.h>

#include <iostream>

namespace {
    
}

namespace tree_sitter_format {

void Traverser::reset(const TraverserContext&) { };

void Traverser::visitLeaf(TSNode, TraverserContext&) { };
void Traverser::preVisitChild(TSNode, uint32_t, TraverserContext&) { };
void Traverser::postVisitChild(TSNode, uint32_t, TraverserContext&) { };

std::vector<Edit> Traverser::traverse(const Document& document, const Style& style) {
    TraverserContext context {
        .document = document,
        .style = style,
    };

    reset(context);
    traverse(document.root(), context);

    return std::move(context.edits);
}

void Traverser::traverse(TSNode node, TraverserContext& context) {
    if (ts_node_is_null(node)) {
        return;
    }

    uint32_t childCount = ts_node_child_count(node);
    if (childCount == 0) {
        visitLeaf(node, context);
    } else {
        for(uint32_t i = 0; i < childCount; i++) {
            preVisitChild(node, i, context);
            traverse(ts_node_child(node, i), context);
            postVisitChild(node, i, context);
        }
    }
}

}
