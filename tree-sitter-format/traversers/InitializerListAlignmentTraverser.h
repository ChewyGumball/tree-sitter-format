#pragma once

#include <tree-sitter-format/traversers/Traverser.h>

namespace tree_sitter_format {

class InitializerListAlignmentTraverser : public Traverser {
protected:
    void preVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) override;
};

}
