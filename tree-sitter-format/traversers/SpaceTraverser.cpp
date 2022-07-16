#include <tree_sitter_format/traversers/SpaceTraverser.h>

#include <tree_sitter_format/Constants.h>

#include <cassert>

using namespace std::literals::string_view_literals;

namespace {
    using namespace tree_sitter_format;
    
    std::string_view ChildFieldName(TSNode node, uint32_t childIndex) {
        const char* name = ts_node_field_name_for_child(node, childIndex);
        if(name == nullptr) {
            return std::string_view();
        } else {
            return name;
        }
    }

    void BinaryOperatorSpacing(TSNode node, uint32_t childIndex, SpaceContext& context) {
        assert(ts_node_symbol(node) == BINARY_EXPRESSION);

        Style::SpacePlacement style = context.style.spacing.binaryOperator;
        if (style == Style::SpacePlacement::Ignore) {
            return;
        }

        std::string_view fieldName = ChildFieldName(node, childIndex);

        if (fieldName == "operator") {
            Position lhs = Position::EndOf(ts_node_child(node, childIndex - 1));
            Position rhs = Position::StartOf(ts_node_child(node, childIndex + 1));

            TSNode child = ts_node_child(node, childIndex);
            Position start = Position::StartOf(child);
            Position end = Position::EndOf(child);

            if (style == Style::SpacePlacement::None || style == Style::SpacePlacement::After) {
                if (lhs < start) {
                    context.edits.push_back(DeleteEdit{.range = Range::Between(lhs, start)});
                }
            }
            else if (lhs.location.row != start.location.row || lhs.location.column != start.location.column - 1) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(lhs, start)});
                context.edits.push_back(InsertEdit{.position = start, .bytes = " "sv});
            }
            
            if (style == Style::SpacePlacement::None || style == Style::SpacePlacement::Before) {
                if (rhs > start) {
                    context.edits.push_back(DeleteEdit{.range = Range::Between(end, rhs)});
                }
            }
            else if (rhs.location.row != end.location.row || rhs.location.column != end.location.column + 1) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(end, rhs)});
                context.edits.push_back(InsertEdit{.position = rhs, .bytes = " "sv});
            }
        }
    }
}

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

void SpaceTraverser::preVisitChild(TSNode node, uint32_t childIndex) {
    if (!context.style.spacing.respace) {
        return;
    }

    TSSymbol symbol = ts_node_symbol(node);
    if (symbol == BINARY_EXPRESSION) {
        BinaryOperatorSpacing(node, childIndex, context);
    }
}

void SpaceTraverser::postVisitChild([[maybe_unused]]TSNode node, [[maybe_unused]]uint32_t childIndex) { }

}
