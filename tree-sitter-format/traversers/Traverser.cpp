#include <tree-sitter-format/traversers/Traverser.h>

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

    TSTreeCursor cursor = ts_tree_cursor_new(document.root());
    traverse(&cursor, context);
    ts_tree_cursor_delete(&cursor);

    return std::move(context.edits);
}

void Traverser::traverse(TSTreeCursor* cursor, TraverserContext& context) {
    TSNode node = ts_tree_cursor_current_node(cursor);
    if (ts_node_is_null(node)) {
        return;
    }

    if (ts_tree_cursor_goto_first_child(cursor)) {
        uint32_t childIndex = 0;
        do {
            preVisitChild(node, childIndex, context);
            traverse(cursor, context);
            postVisitChild(node, childIndex, context);

            childIndex++;
        } while (ts_tree_cursor_goto_next_sibling(cursor));
        assert(ts_tree_cursor_goto_parent(cursor));
    } else {
        visitLeaf(node, context);
    }
}

}
