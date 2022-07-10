#pragma once

#include <tree_sitter/api.h>

namespace tree_sitter_format {

class Traverser {
protected:
    virtual void visitLeaf(TSNode node) = 0;
    virtual void preVisitChild(TSNode node, uint32_t childIndex) = 0;
    virtual void postVisitChild(TSNode node, uint32_t childIndex) = 0;

public:
    void traverse(TSNode node);
};

}
