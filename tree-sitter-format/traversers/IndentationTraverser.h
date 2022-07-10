#pragma once

#include <tree_sitter_format/document/Document.h>
#include <tree_sitter_format/style/Style.h>

namespace tree_sitter_format {

struct IndentationContext {
    uint32_t scope;
    Position previousPosition;

    std::vector<Edit> edits;

    const Style& style;
};

struct IndentationTraverser {
    void visitLeaf(TSNode node, IndentationContext& context);
    void preVisitChild(TSNode node, uint32_t childIndex, IndentationContext& context);
    void postVisitChild(TSNode node, uint32_t childIndex, IndentationContext& context);
};

}
