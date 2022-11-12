#pragma once

#include <tree-sitter-format/traversers/Traverser.h>

namespace tree_sitter_format {

class MultilineCommentReflowTraverser : public Traverser {
protected:
    void visitLeaf(TSNode node, TraverserContext& context) override;
};

}
