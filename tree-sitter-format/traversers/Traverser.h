#pragma once

#include <vector>

#include <tree_sitter/api.h>

#include <tree-sitter-format/document/Edits.h>
#include <tree-sitter-format/document/Document.h>
#include <tree-sitter-format/style/Style.h>

namespace tree_sitter_format {

struct TraverserContext {
    const Document& document;
    const Style& style;
    
    std::vector<Edit> edits;
};

class Traverser {
protected:
    virtual void reset(const TraverserContext& context);

    virtual void visitLeaf(TSNode node, TraverserContext& context);
    virtual void preVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context);
    virtual void postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context);

public:
    virtual ~Traverser() = default;

    std::vector<Edit> traverse(const Document& document, const Style& style);
    void traverse(TSTreeCursor* node, TraverserContext& context);
};

}
