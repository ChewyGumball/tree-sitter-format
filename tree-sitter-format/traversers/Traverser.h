#pragma once

#include <vector>

#include <tree_sitter/api.h>

#include <tree_sitter_format/document/Edits.h>
#include <tree_sitter_format/document/Document.h>
#include <tree_sitter_format/style/Style.h>

namespace tree_sitter_format {

struct TraverserContext {
    const Document& document;
    const Style& style;
    
    std::vector<Edit> edits;
};

class Traverser {
protected:
    virtual void reset(const TraverserContext& context) = 0;

    virtual void visitLeaf(TSNode node, TraverserContext& context) = 0;
    virtual void preVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) = 0;
    virtual void postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) = 0;

public:
    virtual ~Traverser() = default;

    std::vector<Edit> traverse(const Document& document, const Style& style);
    void traverse(TSNode node, TraverserContext& context);
};

}
