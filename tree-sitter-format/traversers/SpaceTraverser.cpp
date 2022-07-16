#include <tree_sitter_format/traversers/SpaceTraverser.h>

#include <cassert>

namespace tree_sitter_format {

SpaceTraverser::SpaceTraverser(const Document& document, const Style& style) : context(SpaceContext{
        .previousPosition = Position {
            .location = TSPoint { .row = 0, .column = 0 },
            .byteOffset = 0,
        },
        .document = document,
        .style = style,
    }) {}

const std::vector<Edit>& SpaceTraverser::edits() const { return context.edits; }

void SpaceTraverser::visitLeaf(TSNode node) {
    if (!context.style.spacing.trimTrailing) {
        return;
    }

    Position currentPosition = Position::StartOf(node);

    if (currentPosition.location.row > context.previousPosition.location.row) {
        Range trailingSpace = context.document.toNextNewLine(context.previousPosition);
        context.edits.push_back(DeleteEdit{ .range = trailingSpace });
    }

    context.previousPosition = Position::EndOf(node);
}

void SpaceTraverser::preVisitChild([[maybe_unused]] TSNode node, [[maybe_unused]] uint32_t childIndex) {

}

void SpaceTraverser::postVisitChild([[maybe_unused]] TSNode node, [[maybe_unused]] uint32_t childIndex) {

}

}
