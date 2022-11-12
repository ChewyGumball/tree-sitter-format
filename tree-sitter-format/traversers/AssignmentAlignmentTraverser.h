#pragma once

#include <tree-sitter-format/traversers/Traverser.h>

namespace tree_sitter_format {

class AssignmentAlignmentTraverser : public Traverser {
protected:
    void postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) override;
};

}
