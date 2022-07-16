#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>

#include <tree_sitter_format/Constants.h>

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
    
void HandleCompoundChild(TSNode node, uint32_t childIndex, BracketExistanceContext& context, Style::Braces style) {
    TSNode child = ts_node_child(node, childIndex);
    TSSymbol symbol = ts_node_symbol(child);
    if (symbol == COMPOUND_STATEMENT) {
        uint32_t compoundChildCount = ts_node_child_count(child);
        if (compoundChildCount == 2) {
            TSNode openBraceNode = ts_node_child(child, 0);
            TSNode closeBraceNode = ts_node_child(child, 1);
            TSNode prev = ts_node_child(node, childIndex - 1);
            TSNode next = ts_node_child(node, childIndex + 1);
            if (ts_node_is_null(next)) {
                next = ts_node_next_sibling(node);
            } 
            TSPoint openPoint = ts_node_start_point(openBraceNode);
            TSPoint closePoint = ts_node_start_point(closeBraceNode);
            TSPoint prevPoint = ts_node_end_point(prev);
            TSPoint nextPoint = ts_node_is_null(next) ? closePoint : ts_node_start_point(next);

            bool sameLinePrev = openPoint.row == prevPoint.row;
            bool sameLineInner = openPoint.row == closePoint.row;
            bool sameLineNext = closePoint.row == nextPoint.row;
            
            bool startNewline = style.openingBrace == Style::NewlinePlacement::Before || style.openingBrace == Style::NewlinePlacement::Surround;
            bool innerNewline = style.openingBrace == Style::NewlinePlacement::Surround || style.closingBrace == Style::NewlinePlacement::Surround || style.closingBrace == Style::NewlinePlacement::Before;
            bool endNewline = style.closingBrace == Style::NewlinePlacement::After || style.closingBrace == Style::NewlinePlacement::Surround;

            bool insertStartNewLine = sameLinePrev && startNewline;
            bool removeStartNewLine = !sameLinePrev && !startNewline;
            bool insertInnerNewLine = sameLineInner && innerNewline;
            bool removeInnerNewLine = !sameLineInner && !innerNewline;
            bool insertEndNewLine = sameLineNext && endNewline;
            bool removeEndNewLine = !sameLineNext && !endNewline;

            if (insertStartNewLine) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(prev), .bytes = "\n"sv});
            } else if (removeStartNewLine) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(prev, openBraceNode)});
            }

            if (insertInnerNewLine) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(openBraceNode), .bytes = "\n"sv});
            } else if (removeInnerNewLine) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(openBraceNode, closeBraceNode)});
            }

            if (insertEndNewLine) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(closeBraceNode), .bytes = "\n"sv});
            }  else if (removeEndNewLine && !ts_node_is_null(next)) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(closeBraceNode, next)});
            }           
        } else if (compoundChildCount == 3 && style.existance == Style::BraceExistance::Remove) {
            // delete the open and close braces because there is only one statement and the style says to remove braces if there is only one statement
            
            TSNode startBrace = ts_node_child(child, 0);
            context.edits.push_back(DeleteEdit {.range = Range::Of(startBrace)});

            TSNode endBrace = ts_node_child(child, 2);
            context.edits.push_back(DeleteEdit {.range = Range::Of(endBrace)});
        }
        else {
            TSNode prev = ts_node_child(node, childIndex - 1);
            TSNode openBraceNode = ts_node_child(child, 0);
            TSNode openNextNode = ts_node_child(child, 1);
            TSNode closePrevNode = ts_node_child(child, compoundChildCount - 2);
            TSNode closeBraceNode = ts_node_child(child, compoundChildCount - 1);
            TSNode next = ts_node_child(node, childIndex + 1);

            if (ts_node_is_null(next)) {
                next = ts_node_next_sibling(node);
            } 

            TSPoint prevPoint = ts_node_end_point(prev);
            TSPoint openPoint = ts_node_start_point(openBraceNode);
            TSPoint openNextPoint = ts_node_start_point(openNextNode);
            TSPoint closePrevPoint = ts_node_end_point(closePrevNode);
            TSPoint closePoint = ts_node_start_point(closeBraceNode);
            TSPoint nextPoint = ts_node_is_null(next) ? closePoint : ts_node_start_point(next);

            bool sameLinePrevOpen = openPoint.row == prevPoint.row;
            bool sameLineNextOpen = openPoint.row == openNextPoint.row;
            bool sameLinePrevClose = closePrevPoint.row == closePoint.row;
            bool sameLineNextClose = closePoint.row == nextPoint.row;
            
            bool startNewlineBefore = style.openingBrace == Style::NewlinePlacement::Before || style.openingBrace == Style::NewlinePlacement::Surround;
            bool startNewlineAfter = style.openingBrace == Style::NewlinePlacement::After || style.openingBrace == Style::NewlinePlacement::Surround;
            bool endNewlineBefore = style.closingBrace == Style::NewlinePlacement::Before || style.closingBrace == Style::NewlinePlacement::Surround;
            bool endNewlineAfter = style.closingBrace == Style::NewlinePlacement::After || style.closingBrace == Style::NewlinePlacement::Surround;

            bool insertStartNewLineBefore = sameLinePrevOpen && startNewlineBefore;
            bool removeStartNewLineBefore = !sameLinePrevOpen && !startNewlineBefore;
            bool insertStartNewLineAfter = sameLineNextOpen && startNewlineAfter;
            bool removeStartNewLineAfter = !sameLineNextOpen && !startNewlineAfter;
            bool insertEndNewLineBefore = sameLinePrevClose && endNewlineBefore;
            bool removeEndNewLineBefore = !sameLinePrevClose && !endNewlineBefore;
            bool insertEndNewLineAfter = sameLineNextClose && endNewlineAfter;
            bool removeEndNewLineAfter = !sameLineNextClose && !endNewlineAfter;

            if (insertStartNewLineBefore) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(prev), .bytes = "\n"sv});
            } else if (removeStartNewLineBefore) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(prev, openBraceNode)});
            }
            
            if (insertStartNewLineAfter) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(openBraceNode), .bytes = "\n"sv});
            } else if (removeStartNewLineAfter) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(openBraceNode, openNextNode)});
            }
            
            if (insertEndNewLineBefore) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(closePrevNode), .bytes = "\n"sv});
            } else if (removeEndNewLineBefore) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(closePrevNode, closeBraceNode)});
            }
            
            if (insertEndNewLineAfter) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(closeBraceNode), .bytes = "\n"sv});
            } else if (removeEndNewLineAfter && !ts_node_is_null(next)) {
                context.edits.push_back(DeleteEdit{.range = Range::Between(closeBraceNode, next)});
            }
                      
        }
    } else if (style.existance == Style::BraceExistance::Require) {
        // Insert open and close braces because the style requires they exist but this block doesn't have them

        TSNode prev = ts_node_child(node, childIndex - 1);

        TSPoint prevPoint = ts_node_end_point(prev);
        TSPoint point = ts_node_start_point(child);

        bool sameLinePrev = prevPoint.row == point.row;

        std::string_view openingBrace;
        switch(style.openingBrace) {
            case Style::NewlinePlacement::Before: openingBrace = "\n{"sv; break;
            case Style::NewlinePlacement::After: {
                openingBrace = sameLinePrev ? "{\n"sv : "{"sv;
                break;
            }
            case Style::NewlinePlacement::Surround: {
                openingBrace = sameLinePrev ? "\n{\n"sv : "\n{"sv;
                break;
            }
            case Style::NewlinePlacement::Ignore: openingBrace = "{"sv; break;
            default: openingBrace = "{"sv; break;                
        }

        context.edits.push_back(InsertEdit{.position = Position::EndOf(prev), .bytes = openingBrace});

        TSNode next = ts_node_child(node, childIndex + 1);

        if (ts_node_is_null(next)) {
            next = ts_node_next_sibling(node);
        } 
        
        bool sameLineNext = true;
        if (!ts_node_is_null(next)) {
            TSPoint nextPoint = ts_node_start_point(next);
            sameLineNext = nextPoint.row == point.row;
        }

        std::string_view closingBrace = "\n}\n"sv;
        switch(style.closingBrace) {
            case Style::NewlinePlacement::Before: closingBrace = "\n}"sv; break;
            case Style::NewlinePlacement::After: {
                closingBrace = sameLineNext ? "}\n"sv : "}"sv;
                break;
            }
            case Style::NewlinePlacement::Surround: {
                closingBrace = sameLineNext ? "\n}\n"sv : "\n}"sv;
                break;
            }
            case Style::NewlinePlacement::Ignore: closingBrace = "}"sv; break;
            default: closingBrace = "}"sv; break;                
        }

        context.edits.push_back(InsertEdit{.position = Position::EndOf(child), .bytes = closingBrace});
    }
}

void IfStatementEdits(TSNode node, uint32_t childIndex, BracketExistanceContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "consequence" && fieldName != "alternative") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.ifStatements);
}

void WhileLoopEdits(TSNode node, uint32_t childIndex, BracketExistanceContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.whileLoops);
}

void DoWhileLoopEdits(TSNode node, uint32_t childIndex, BracketExistanceContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.doWhileLoops);
}

void ForLoopEdits(TSNode node, uint32_t childIndex, BracketExistanceContext& context) {
    // For Loops don't have a named body, its just the last child...
    if (childIndex != ts_node_child_count(node) - 1) {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.forLoops);
}

void ForRangeLoopEdits(TSNode node, uint32_t childIndex, BracketExistanceContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.forLoops);
}
void CaseStatementEdits(TSNode node, uint32_t childIndex, BracketExistanceContext& context) {
    // Case statements are defined as: ('case' {expression} | 'default) ':' {stuff}+ 
    // so we need to find where the ':' is, and everything after that is what would be the
    // target of our brace work.
    const Style::Braces& style = context.style.braces.caseStatements;

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
        if (style.existance == Style::BraceExistance::Require) {

            if (childIndex == firstBodyStatement) {
                TSNode prev = ts_node_child(node, childIndex - 1);
                TSNode child = ts_node_child(node, childIndex);

                TSPoint prevPoint = ts_node_end_point(prev);
                TSPoint point = ts_node_start_point(child);

                bool sameLinePrev = prevPoint.row == point.row;                    

                std::string_view openingBrace;
                switch(style.openingBrace) {
                    case Style::NewlinePlacement::Before: openingBrace = "\n{"sv; break;
                    case Style::NewlinePlacement::After: {
                        openingBrace = sameLinePrev ? "{\n"sv : "{"sv;
                        break;
                    }
                    case Style::NewlinePlacement::Surround: {
                        openingBrace = sameLinePrev ? "\n{\n"sv : "\n{"sv;
                        break;
                    }
                    case Style::NewlinePlacement::Ignore: openingBrace = "{"sv; break;
                    default: openingBrace = "{"sv; break;                
                }

                context.edits.push_back(InsertEdit{.position = Position::EndOf(prev), .bytes = openingBrace});
            } else if (childIndex == lastChildIndex) {
                TSNode child = ts_node_child(node, childIndex);            
                TSNode next = ts_node_child(node, childIndex + 1);

                TSPoint point = ts_node_start_point(child);

                if (ts_node_is_null(next)) {
                    next = ts_node_next_sibling(node);
                } 
                
                
                bool sameLineNext = true;
                if (!ts_node_is_null(next)) {
                    TSPoint nextPoint = ts_node_start_point(next);
                    sameLineNext = nextPoint.row == point.row;
                }
                
                std::string_view closingBrace = "\n}\n"sv;
                switch(style.closingBrace) {
                    case Style::NewlinePlacement::Before: closingBrace = "\n}"sv; break;
                    case Style::NewlinePlacement::After: {
                        closingBrace = sameLineNext ? "}\n"sv : "}"sv;
                        break;
                    }
                    case Style::NewlinePlacement::Surround: {
                        closingBrace = sameLineNext ? "\n}\n"sv : "\n}"sv;
                        break;
                    }
                    case Style::NewlinePlacement::Ignore: closingBrace = "}"sv; break;
                    default: closingBrace = "}"sv; break;                
                }

                context.edits.push_back(InsertEdit{.position = Position::EndOf(child), .bytes = closingBrace});
            }
        }
    }
}

void SwitchStatementEdits(TSNode node, uint32_t childIndex, BracketExistanceContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "body") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, context.style.braces.switchStatements);
}
}

namespace tree_sitter_format {
BracketExistanceTraverser::BracketExistanceTraverser(const Style& style) : context({.style = style}) {}

const std::vector<Edit>& BracketExistanceTraverser::edits() const { return context.edits; }

void BracketExistanceTraverser::visitLeaf([[maybe_unused]] TSNode node) { }

void BracketExistanceTraverser::preVisitChild(TSNode node, uint32_t childIndex) {
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
void BracketExistanceTraverser::postVisitChild([[maybe_unused]] TSNode node, [[maybe_unused]] uint32_t childIndex) { }
}
