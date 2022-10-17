#include <tree_sitter_format/traversers/SpaceTraverser.h>

#include <tree_sitter_format/Constants.h>
#include <tree_sitter_format/Util.h>

#include <cassert>

using namespace std::literals::string_view_literals;

namespace {
    using namespace tree_sitter_format;

    void EnsureSpacing(TSNode currentNode, TSNode nextNode, Style::Whitespace style, TraverserContext& context) {
        if (style == Style::Whitespace::Ignore) {
            return;
        }

        const Position lhs = Position::EndOf(currentNode);
        const Position rhs = Position::StartOf(nextNode);

        const bool onSameLine = lhs.location.row == rhs.location.row;

        if (onSameLine && style == Style::Whitespace::Newline) {
            context.edits.push_back(InsertEdit{.position = lhs, .bytes = context.style.newLineString()});
        } else if (!onSameLine && style != Style::Whitespace::Newline) {
            context.edits.push_back(DeleteEdit{.range = Range::Between(lhs, rhs)});
            if (style == Style::Whitespace::Space) {
                context.edits.push_back(InsertEdit {.position = rhs, .bytes = " "sv});
            }
        } else if (onSameLine && style == Style::Whitespace::Space) {
            context.edits.push_back(DeleteEdit{.range = Range::Between(lhs, rhs)});
            context.edits.push_back(InsertEdit {.position = rhs, .bytes = " "sv});            
        }
    }

    void EnsureSpacing(TSNode currentNode, TSNode previousNode, TSNode nextNode, Style::WhitespacePlacement style, TraverserContext& context) {
        EnsureSpacing(previousNode, currentNode, style.before, context);
        EnsureSpacing(currentNode, nextNode, style.after, context);
    }

    void EnsureSpacing(TSNode parent, uint32_t childIndex, Style::WhitespacePlacement style, TraverserContext& context) {
        TSNode prev = ts_node_child(parent, childIndex - 1);
        TSNode child = ts_node_child(parent, childIndex);
        TSNode next = ts_node_child(parent, childIndex + 1);

        EnsureSpacing(child, prev, next, style, context);
    }

    void CommaExpressionSpacing(TSNode node, Style::WhitespacePlacement style, TraverserContext& context) {
        EnsureSpacing(node, 1, style, context);

        TSNode rightSide = ts_node_child(node, 2);
        if (ts_node_symbol(rightSide) == COMMA_EXPRESSION) {
            CommaExpressionSpacing(rightSide, style, context);
        }
    }

    void ForLoopStatementSpacing(TSNode node, uint32_t childIndex, TraverserContext& context) {
        assert(ts_node_symbol(node) == FOR_LOOP);

        std::string_view fieldName = ChildFieldName(node, childIndex);

        if (childIndex == 1) {
            // Opening Parenthesis
            const Style::WhitespacePlacement style = context.style.spacing.forLoops.parentheses.opening;
            EnsureSpacing(node, childIndex, style, context);
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
                    const Style::WhitespacePlacement style = context.style.spacing.forLoops.commas;
                    EnsureSpacing(child, i, style, context);
                }

                // handle the ending semicolon (the last child of the child)
                const Style::WhitespacePlacement style = context.style.spacing.forLoops.semicolons;
                
                TSNode prev = ts_node_child(child, childChildCount - 2);
                TSNode semicolon = ts_node_child(child, childChildCount - 1);
                TSNode next = ts_node_child(node, childIndex + 1);
                EnsureSpacing(semicolon, prev, next, style, context);

            } else {
                if (childSymbol == COMMA_EXPRESSION) {
                    // handle commas
                    const Style::WhitespacePlacement style = context.style.spacing.forLoops.commas;
                    CommaExpressionSpacing(child, style, context);
                }

                // handle ending semicolon (its childIndex + 1 now)
                const Style::WhitespacePlacement style = context.style.spacing.forLoops.semicolons;
                EnsureSpacing(node, childIndex + 1, style, context);
            }
        } else if (fieldName == "condition") {
            // handle ending semicolon (its childIndex + 1 now)   
            const Style::WhitespacePlacement style = context.style.spacing.forLoops.semicolons;
            EnsureSpacing(node, childIndex + 1, style, context);
        } else if (fieldName == "update") {
            // We could have an arbitrary expression, or a comma expression
            TSNode child = ts_node_child(node, childIndex);
            TSSymbol childSymbol = ts_node_symbol(child);

            if (childSymbol == COMMA_EXPRESSION) {
                // handle commas
                const Style::WhitespacePlacement style = context.style.spacing.forLoops.commas;
                CommaExpressionSpacing(child, style, context);
            }

            // handle closing paranthesis (its childIndex + 1 now)            
            const Style::WhitespacePlacement style = context.style.spacing.forLoops.parentheses.closing;
            EnsureSpacing(node, childIndex + 1, style, context);
        } else if (fieldName == "body") {
            // Body
            TSNode child = ts_node_child(node, childIndex);
            TSSymbol childSymbol = ts_node_symbol(child);

            if (childSymbol == COMPOUND_STATEMENT) {
                // handle braces
            }
        }
    }

    void BinaryOperatorSpacing(TSNode node, uint32_t childIndex, TraverserContext& context) {
        assert(ts_node_symbol(node) == BINARY_EXPRESSION);

        std::string_view fieldName = ChildFieldName(node, childIndex);
        if (fieldName == "operator") {        
            Style::WhitespacePlacement style = context.style.spacing.binaryOperator;

            TSNode lhs = ts_node_child(node, childIndex - 1);
            TSNode child = ts_node_child(node, childIndex);
            TSNode rhs = ts_node_child(node, childIndex + 1);

            EnsureSpacing(child, lhs, rhs, style, context);
        }
    }

    void BitFieldSpacing(TSNode node, uint32_t childIndex, TraverserContext& context) {
        assert(ts_node_symbol(node) == FIELD_DECLARATION);
        TSNode child = ts_node_child(node, childIndex);

        if (ts_node_symbol(child) != BITFIELD_CLAUSE) {
            return;
        }

        Style::WhitespacePlacement style = context.style.spacing.bitFields.colon;

        TSNode previousNode = ts_node_prev_sibling(child);
        EnsureSpacing(previousNode, child, style.before, context);

        TSNode colon = ts_node_child(child, 0);
        TSNode nextNode = ts_node_child(child, 1);

        EnsureSpacing(colon, nextNode, style.after, context);
    }
}

namespace tree_sitter_format {
    
void SpaceTraverser::reset(const TraverserContext& context) {
    previousPosition = Position::StartOf(context.document.root());
}

void SpaceTraverser::visitLeaf(TSNode node, TraverserContext& context) {
    if (!context.style.spacing.trimTrailing) {
        return;
    }

    Position currentPosition = Position::StartOf(node);

    if (currentPosition.location.row > previousPosition.location.row) {
        Range trailingSpace = context.document.toNextNewLine(previousPosition);
        context.edits.push_back(DeleteEdit{ .range = trailingSpace });
    }

    previousPosition = Position::EndOf(node);
}

void SpaceTraverser::preVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    if (!context.style.spacing.respace) {
        return;
    }

    TSSymbol symbol = ts_node_symbol(node);
    if (symbol == BINARY_EXPRESSION) {
        BinaryOperatorSpacing(node, childIndex, context);
    } else if (symbol == FOR_LOOP) {
        ForLoopStatementSpacing(node, childIndex, context);
    } else if (symbol == FIELD_DECLARATION) {
        BitFieldSpacing(node, childIndex, context);
    }
}

void SpaceTraverser::postVisitChild(
    [[maybe_unused]]TSNode node,
    [[maybe_unused]]uint32_t childIndex,
    [[maybe_unused]]TraverserContext& context) {
}

}
