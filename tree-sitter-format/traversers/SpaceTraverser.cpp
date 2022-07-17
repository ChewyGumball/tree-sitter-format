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

    void EnsureSpacing(TSNode currentNode, TSNode previousNode, TSNode nextNode, Style::SpacePlacement style, std::vector<Edit>& edits) {
        if (style == Style::SpacePlacement::Ignore) {
            return;
        }


        Position lhs = Position::EndOf(previousNode);
        Position rhs = Position::StartOf(nextNode);

        Position start = Position::StartOf(currentNode);
        Position end = Position::EndOf(currentNode);

        if (style == Style::SpacePlacement::None || style == Style::SpacePlacement::After) {
            if (lhs < start) {
                edits.push_back(DeleteEdit{.range = Range::Between(lhs, start)});
            }
        }
        else if (lhs.location.row != start.location.row || lhs.location.column != start.location.column - 1) {
            edits.push_back(DeleteEdit{.range = Range::Between(lhs, start)});
            edits.push_back(InsertEdit{.position = start, .bytes = " "sv});
        }
        
        if (style == Style::SpacePlacement::None || style == Style::SpacePlacement::Before) {
            if (rhs > start) {
                edits.push_back(DeleteEdit{.range = Range::Between(end, rhs)});
            }
        }
        else if (rhs.location.row != end.location.row || rhs.location.column != end.location.column + 1) {
            edits.push_back(DeleteEdit{.range = Range::Between(end, rhs)});
            edits.push_back(InsertEdit{.position = rhs, .bytes = " "sv});
        }
    }

    void EnsureSpacing(TSNode parent, uint32_t childIndex, Style::SpacePlacement style, std::vector<Edit>& edits) {
        TSNode prev = ts_node_child(parent, childIndex - 1);
        TSNode child = ts_node_child(parent, childIndex);
        TSNode next = ts_node_child(parent, childIndex + 1);

        EnsureSpacing(child, prev, next, style, edits);
    }

    void CommaExpressionSpacing(TSNode node, Style::SpacePlacement style, SpaceContext& context) {
        EnsureSpacing(node, 1, style, context.edits);

        TSNode rightSide = ts_node_child(node, 2);
        if (ts_node_symbol(rightSide) == COMMA_EXPRESSION) {
            CommaExpressionSpacing(rightSide, style, context);
        }
    }

    void ForLoopStatementSpacing(TSNode node, uint32_t childIndex, SpaceContext& context) {
        assert(ts_node_symbol(node) == FOR_LOOP);

        std::string_view fieldName = ChildFieldName(node, childIndex);

        if (childIndex == 1) {
            // Opening Parenthesis
            const Style::SpacePlacement style = context.style.spacing.forLoops.openingParenthesis;
            EnsureSpacing(node, childIndex, style, context.edits);
        } else if (fieldName == "initializer") {
            // We could have a declaration, or an arbitrary expression, or a comma expression
            TSNode child = ts_node_child(node, childIndex);
            TSSymbol childSymbol = ts_node_symbol(child);

            if (childSymbol == DECLARATION) {
                uint32_t childChildCount = ts_node_child_count(child);

                // If there are more than 2 children, its comma separated, so 
                // handle commas in declaration. Each non comma is supposed to
                // be labeled as "declarator", but there seems to be a bug, and
                // some commas are labeled that instead... so don't use labels...
                for(uint32_t i = 2; i < childChildCount - 1; i += 2) {
                    const Style::SpacePlacement style = context.style.spacing.forLoops.commas;
                    EnsureSpacing(child, i, style, context.edits);
                }

                // handle the ending semicolon (the last child of the child)
                const Style::SpacePlacement style = context.style.spacing.forLoops.semicolons;
                
                TSNode prev = ts_node_child(child, childChildCount - 2);
                TSNode semicolon = ts_node_child(child, childChildCount - 1);
                TSNode next = ts_node_child(node, childIndex + 1);
                EnsureSpacing(semicolon, prev, next, style, context.edits);

            } else {
                if (childSymbol == COMMA_EXPRESSION) {
                    // handle commas
                    const Style::SpacePlacement style = context.style.spacing.forLoops.commas;
                    CommaExpressionSpacing(child, style, context);
                }

                // handle ending semicolon (its childIndex + 1 now)
                const Style::SpacePlacement style = context.style.spacing.forLoops.semicolons;
                EnsureSpacing(node, childIndex + 1, style, context.edits);
            }
        } else if (fieldName == "condition") {
            // handle ending semicolon (its childIndex + 1 now)   
            const Style::SpacePlacement style = context.style.spacing.forLoops.semicolons;
            EnsureSpacing(node, childIndex + 1, style, context.edits);
        } else if (fieldName == "update") {
            // We could have an arbitrary expression, or a comma expression
            TSNode child = ts_node_child(node, childIndex);
            TSSymbol childSymbol = ts_node_symbol(child);

            if (childSymbol == COMMA_EXPRESSION) {
                // handle commas
                const Style::SpacePlacement style = context.style.spacing.forLoops.commas;
                CommaExpressionSpacing(child, style, context);
            }

            // handle closing paranthesis (its childIndex + 1 now)            
            const Style::SpacePlacement style = context.style.spacing.forLoops.closingParenthesis;
            EnsureSpacing(node, childIndex + 1, style, context.edits);
        } else if (childIndex == ts_node_child_count(node) - 1) {
            // Body
            TSNode child = ts_node_child(node, childIndex);
            TSSymbol childSymbol = ts_node_symbol(child);

            if (childSymbol == COMPOUND_STATEMENT) {
                // handle braces
            }
        }
    }

    void BinaryOperatorSpacing(TSNode node, uint32_t childIndex, SpaceContext& context) {
        assert(ts_node_symbol(node) == BINARY_EXPRESSION);

        std::string_view fieldName = ChildFieldName(node, childIndex);
        if (fieldName == "operator") {        
            Style::SpacePlacement style = context.style.spacing.binaryOperator;

            TSNode lhs = ts_node_child(node, childIndex - 1);
            TSNode child = ts_node_child(node, childIndex);
            TSNode rhs = ts_node_child(node, childIndex + 1);

            EnsureSpacing(child, lhs, rhs, style, context.edits);
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
    } else if (symbol == FOR_LOOP) {
        ForLoopStatementSpacing(node, childIndex, context);
    }
}

void SpaceTraverser::postVisitChild([[maybe_unused]]TSNode node, [[maybe_unused]]uint32_t childIndex) { }

}
