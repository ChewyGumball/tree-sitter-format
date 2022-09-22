#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>

#include <tree_sitter_format/Constants.h>

#include <assert.h>

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
    
void HandleCompoundChild(TSNode node, uint32_t childIndex, TraverserContext& context, Style::BraceExistance style) {
    TSNode child = ts_node_child(node, childIndex);
    TSSymbol symbol = ts_node_symbol(child);
    if (symbol == COMPOUND_STATEMENT) {
        uint32_t compoundChildCount = ts_node_child_count(child);
        // Delete the open and close braces if there is only one statement and the style says to remove braces
        // if there is only one statement            
        if (compoundChildCount == 3 && style == Style::BraceExistance::Remove) {
            TSNode startBrace = ts_node_child(child, 0);
            context.edits.push_back(DeleteEdit {.range = Range::Of(startBrace)});

            TSNode endBrace = ts_node_child(child, 2);
            context.edits.push_back(DeleteEdit {.range = Range::Of(endBrace)});
        }
    } else if (style == Style::BraceExistance::Require) {
        // Insert open and close braces because the style requires they exist but this block doesn't have them
        context.edits.push_back(InsertEdit{.position = Position::StartOf(child), .bytes = "{"sv});
        context.edits.push_back(InsertEdit{.position = Position::EndOf(child), .bytes = "}"sv});
    }
}

void IfStatementEdits(TSNode node, uint32_t childIndex, TraverserContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "consequence" && fieldName != "alternative") {
        return;
    }

    // There is a special case for if statement alternatives (aka, the "else" part). If
    // the alternative is a single statement, and that statement is an "if" statement,
    // don't increase the scope because it is an "else if". The grammar considers the 
    // "if" part of an "else if" to be a child node, but we don't want to add braces
    // around the if
    //
    // IE, we want:
    // if (true) {
    //     int a;
    // } else if (false) {
    //     int b;
    // }
    //
    // not:
    //  if (true) {
    //     int a;
    //  } else {if (false) {
    //     int b;
    //  }}
    if (fieldName == "alternative") {
        TSNode child = ts_node_child(node, childIndex);
        TSSymbol childSymbol = ts_node_symbol(child);

        if (childSymbol == IF_STATEMENT) {
            return;
        }
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.ifStatements);
}

void WhileLoopEdits(TSNode node, uint32_t childIndex, TraverserContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.whileLoops);
}

void DoWhileLoopEdits(TSNode node, uint32_t childIndex, TraverserContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.doWhileLoops);
}

void ForLoopEdits(TSNode node, uint32_t childIndex, TraverserContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.forLoops);
}

void ForRangeLoopEdits(TSNode node, uint32_t childIndex, TraverserContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.forLoops);
}

void CaseStatementEdits(TSNode node, uint32_t childIndex, TraverserContext& context) {
    // Case statements are defined as: ('case' {expression} | 'default) ':' {stuff}+ 
    // so we need to find where the ':' is, and everything after that is what would be the
    // target of our brace work.
    const Style::BraceExistance& style = context.style.braces.caseStatements;

    std::string_view secondChildFieldName = ChildFieldName(node, 1);
    bool isDefaultCase = secondChildFieldName != "value";
    
    uint32_t firstBodyStatement = isDefaultCase ? 2 : 3;
    uint32_t lastChildIndex = ts_node_child_count(node) - 1;

    bool singleBodyStatement = firstBodyStatement == lastChildIndex;

    // If there is only one child after the colon, we can treat it just like a loop body.
    if (singleBodyStatement) {
        if (childIndex == lastChildIndex) {
            HandleCompoundChild(node, childIndex, context, style);
        }
    } else {
        // If existance is remove, well, we don't have a compound statement, so theres nothing to remove.
        // If existance is ignore, well, don't do anything.
        // If existance is require, we need to add braces here because its not a single compound statement,
        // and therefore the braces don't exist.
        if (style == Style::BraceExistance::Require) {
            TSNode child = ts_node_child(node, childIndex);

            if (childIndex == firstBodyStatement) {
                context.edits.push_back(InsertEdit{.position = Position::StartOf(child), .bytes = "{"sv});
            } else if (childIndex == lastChildIndex) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(child), .bytes = "}"sv});
            }
        }
    }
}

void SwitchStatementEdits(TSNode node, uint32_t childIndex, TraverserContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.switchStatements);
}
}

namespace tree_sitter_format {
void BracketExistanceTraverser::reset([[maybe_unused]]const TraverserContext& context) { }
void BracketExistanceTraverser::visitLeaf([[maybe_unused]] TSNode node, [[maybe_unused]]TraverserContext& context) { }

void BracketExistanceTraverser::preVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    TSSymbol symbol = ts_node_symbol(node);

    if (symbol == IF_STATEMENT) {
        IfStatementEdits(node, childIndex, context);
    } else if (symbol == WHILE_LOOP) {
        WhileLoopEdits(node, childIndex, context);
    } else if (symbol == DO_WHILE_LOOP) {
        DoWhileLoopEdits(node, childIndex, context);
    } else if (symbol == FOR_LOOP) {
        ForLoopEdits(node, childIndex, context);
    } else if (symbol == FOR_RANGE_LOOP) {
        ForRangeLoopEdits(node, childIndex, context);
    } else if (symbol == SWITCH_STATEMENT) {
        SwitchStatementEdits(node, childIndex, context);
    } else if (symbol == CASE_STATEMENT) {
        CaseStatementEdits(node, childIndex, context);
    }
}
void BracketExistanceTraverser::postVisitChild([[maybe_unused]] TSNode node, [[maybe_unused]] uint32_t childIndex, [[maybe_unused]]TraverserContext& context) { }
}
