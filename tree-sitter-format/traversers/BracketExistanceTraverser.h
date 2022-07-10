#pragma once

#include <tree_sitter_format/traversers/Traverser.h>

#include <tree_sitter_format/document/Document.h>
#include <tree_sitter_format/style/Style.h>

namespace tree_sitter_format {

struct BracketExistanceContext {
    std::vector<Edit> edits;

    const Style& style;
};

class BracketExistanceTraverser : public Traverser {
public:
    BracketExistanceTraverser(const Style& style);

    const std::vector<Edit>& edits() const;

private:
    BracketExistanceContext context;

protected:
    void visitLeaf(TSNode node) override;
    void preVisitChild(TSNode node, uint32_t childIndex) override;
    void postVisitChild(TSNode node, uint32_t childIndex) override;
};

}
