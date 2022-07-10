#pragma once

#include <tree_sitter_format/traversers/Traverser.h>

#include <tree_sitter_format/document/Document.h>
#include <tree_sitter_format/style/Style.h>

namespace tree_sitter_format {

struct IndentationContext {
    uint32_t scope;
    Position previousPosition;

    std::vector<Edit> edits;

    const Style& style;
};

class IndentationTraverser : public Traverser {    
public:
    IndentationTraverser(const Style& style);

    const std::vector<Edit>& edits() const;

private:
    IndentationContext context;

protected:
    void visitLeaf(TSNode node) override;
    void preVisitChild(TSNode node, uint32_t childIndex) override;
    void postVisitChild(TSNode node, uint32_t childIndex) override;
};

}
