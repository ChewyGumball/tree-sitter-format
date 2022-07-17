#pragma once

#include <tree_sitter_format/traversers/Traverser.h>

namespace tree_sitter_format {

class ParseTraverser : public Traverser {
private:
    uint32_t index = 0;
    uint32_t scope = 0;

protected:
    void visitLeaf(TSNode node) override;
    void preVisitChild(TSNode node, uint32_t childIndex) override;
    void postVisitChild(TSNode node, uint32_t childIndex) override;
};

}
