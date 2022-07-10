#include <tree_sitter_format/style/Style.h>
#include <tree_sitter_format/document/Document.h>

#include <tree_sitter/api.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>

using namespace std::literals::string_view_literals;

#include <cassert>

extern "C" {
const TSLanguage* tree_sitter_cpp(void);
}

using namespace tree_sitter_format;

const std::filesystem::path inputFileName = "C:/Users/bpdun/Desktop/test/test.cpp";
const std::filesystem::path outputFileName = "C:/Users/bpdun/Desktop/test/test.cpp.out";

TSSymbol IF_STATEMENT;
TSSymbol FOR_LOOP;
TSSymbol FOR_RANGE_LOOP;
TSSymbol WHILE_LOOP;
TSSymbol DO_WHILE_LOOP;
TSSymbol FUNCTION_DEFINITION;

// Regular code block
TSSymbol COMPOUND_STATEMENT;
// The block for namespaces and externs
TSSymbol DECLARATION_LIST;
// The block for enums
TSSymbol ENUMERATOR_LIST;
// The block for structs or classes
TSSymbol FIELD_DECLARATION_LIST;
// The block for braced initialiation (designated initialization, struct initialization, etc)
TSSymbol INITIALIZER_LIST;

TSSymbol ERROR;

std::string_view ToStr(TSNode node, std::string_view text) {
    if (ts_node_is_null(node)) {
        return "(null)";
    }

    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);

    return text.substr(start, end - start);
}

struct ScopeContext {
    uint32_t depth = 0;
    uint32_t scope = 0;
    std::ostream& output;
    
    Position previousPosition;

    const Document& document;
    const Style& style;
};

void traverseChild(TSNode node, ScopeContext& context) {
    std::string token = context.document.contentsAt(Range::Of(node));

    TSPoint currentPoint = ts_node_start_point(node);
    for(uint32_t i = context.previousPosition.location.row; i < currentPoint.row; i++) {
        context.output << std::endl;
    }

    if (context.previousPosition.location.row != currentPoint.row) {
        for(uint32_t i = 0; i < context.scope; i++) {
            context.output << context.style.indentationString();
        }
    } else {
        Range whiteSpaceRange {
            .start = context.previousPosition,
            .end = Position::StartOf(node),
        };

        context.output << context.document.contentsAt(whiteSpaceRange);
    }

    context.output << token << std::flush;

    context.previousPosition = Position::EndOf(node);
}

std::string_view ChildFieldName(TSNode node, uint32_t childIndex) {
    const char* name = ts_node_field_name_for_child(node, childIndex);
    if(name == nullptr) {
        return std::string_view();
    } else {
        return name;
    }
}

enum class ScopeChange { None, IncreaseBefore, DecreaseAfter, Both };

template<size_t COUNT>
ScopeChange NonCompoundBodyScopeChange(TSNode node, uint32_t childIndex, const std::string_view (&allowedFieldNames)[COUNT], Style::Indentation allowedIndentation) {
    std::string_view fieldName = ChildFieldName(node, childIndex);
    for(auto allowedName : allowedFieldNames) {
        if (fieldName == allowedName) {
            TSNode child = ts_node_child(node, childIndex);
            TSSymbol childSymbol = ts_node_symbol(child);

            // Compound statements handle their own indentation so they can be handled uniformly
            if (childSymbol == COMPOUND_STATEMENT) {
                return ScopeChange::None;
            }
            
            if (allowedIndentation != Style::Indentation::None) {
                // Since this isn't a compound statement, its a single statement body
                // so we increase before, and decrease after
                return ScopeChange::Both;
            }
        }
    }

    return ScopeChange::None;
}

ScopeChange IfStatementScopeChange(TSNode node, uint32_t childIndex, const ScopeContext& context) {
    assert(ts_node_symbol(node) == IF_STATEMENT);
    return NonCompoundBodyScopeChange(node, childIndex, {"consequence"sv, "alternative"sv}, context.style.indentation.ifStatements);
}

ScopeChange WhileLoopScopeChange(TSNode node, uint32_t childIndex, const ScopeContext& context) {
    assert(ts_node_symbol(node) == WHILE_LOOP);
    return NonCompoundBodyScopeChange(node, childIndex, {"body"sv}, context.style.indentation.whileLoops);
}

ScopeChange DoWhileLoopScopeChange(TSNode node, uint32_t childIndex, const ScopeContext& context) {
    assert(ts_node_symbol(node) == DO_WHILE_LOOP);
    return NonCompoundBodyScopeChange(node, childIndex, {"body"sv}, context.style.indentation.whileLoops);
}

ScopeChange CompoundStatementScopeChange(TSNode node, TSNode parent, uint32_t childIndex, const ScopeContext& context) {
    // Compound statements are '{', (statement), ... , '}'
    // There may be no (statement) nodes, ie an empty block

    uint32_t childCount = ts_node_child_count(node);
    if (childCount == 2) {
        return ScopeChange::None;
    }

    TSSymbol parentSymbol = ts_node_symbol(parent);
    Style::Indentation style = Style::Indentation::None;
    if (parentSymbol == IF_STATEMENT) {
        style = context.style.indentation.ifStatements;
    } else if (parentSymbol == FOR_LOOP || parentSymbol == FOR_RANGE_LOOP) {
        style = context.style.indentation.forLoops;
    } else if (parentSymbol == WHILE_LOOP) {
        style = context.style.indentation.whileLoops;
    } else if (parentSymbol == DO_WHILE_LOOP) {
        style = context.style.indentation.doWhileLoops;
    } else if (parentSymbol == FUNCTION_DEFINITION) {
        style = context.style.indentation.functionDefinitions;
    }
    // TODO: add case blocks
    // TODO: add switch blocks

    // We are the opening brace
    if (childIndex == 0) {
        switch(style) {
            case Style::Indentation::BracesIndented:
            case Style::Indentation::BothIndented:
                return ScopeChange::IncreaseBefore;
            case Style::Indentation::BodyIndented:
            case Style::Indentation::None:
            default:
                return ScopeChange::None;        
        }
    } 
    
    // We are the first statement after the opening brace
    if (childIndex == 1) {
        switch(style) {
            case Style::Indentation::BodyIndented:
            case Style::Indentation::BothIndented:
            // If this is the only non brace child, we increment before, and decrement after
            // otherwise, we increment before, and the decrement will happen for the child
            // that comes before the end brace 
                return childCount == 3 ? ScopeChange::Both : ScopeChange::IncreaseBefore;
            case Style::Indentation::BracesIndented:
            case Style::Indentation::None:
            default:
                return ScopeChange::None;        
        }
    } 
    
    // We are the last statement before the closing brace
    if (childIndex == childCount - 2) {
        switch(style) {
            case Style::Indentation::BodyIndented:
            case Style::Indentation::BothIndented:
            // If this is the only non brace child, we increment before, and decrement after
            // otherwise, we decrement after, and the increment happened for the child that
            // came after the start brace
                return childCount == 3 ? ScopeChange::Both : ScopeChange::DecreaseAfter;
            case Style::Indentation::BracesIndented:
            case Style::Indentation::None:
            default:
                return ScopeChange::None;        
        }
    } 
    
    // We are the closing brace
    if (childIndex == childCount - 1) {
        switch(style) {
            case Style::Indentation::BracesIndented:
            case Style::Indentation::BothIndented:
                return ScopeChange::DecreaseAfter;
            case Style::Indentation::BodyIndented:
            case Style::Indentation::None:
            default:
                return ScopeChange::None;        
        }
    }

    // This is just a statement in the middle of a block, nothing special happens with
    // indentation here
    return ScopeChange::None;
}

ScopeChange ScopeChangeForChild(TSNode node, TSNode parent, uint32_t childIndex, const ScopeContext& context) {
    TSSymbol symbol = ts_node_symbol(node);

    if (symbol == IF_STATEMENT) {
        return IfStatementScopeChange(node, childIndex, context);
    } else if (symbol == WHILE_LOOP) {
        return WhileLoopScopeChange(node, childIndex, context);
    } else if (symbol == DO_WHILE_LOOP) {
        return DoWhileLoopScopeChange(node, childIndex, context);
    } else if (symbol == COMPOUND_STATEMENT) {
        return CompoundStatementScopeChange(node, parent, childIndex, context);
    }

    return ScopeChange::None;
}

void traverse(TSNode node, TSNode parent, ScopeContext& context) {
    if (ts_node_is_null(node)) {
        return;
    }

    // DEBUG print parse tree
    for(uint32_t i = 0; i < context.depth; i++) {
        std::cout << "    ";
    }
    std::cout << ts_node_type(node) << std::endl;

    uint32_t childCount = ts_node_child_count(node);
    if (childCount == 0) {
        traverseChild(node, context);
    } else {
        context.depth++;

        for(uint32_t i = 0; i < childCount; i++) {
            ScopeChange scopeChange = ScopeChangeForChild(node, parent, i, context);
            if (scopeChange == ScopeChange::IncreaseBefore || scopeChange == ScopeChange::Both) {
                context.scope++;
            }

            traverse(ts_node_child(node, i), node, context);

            if (scopeChange == ScopeChange::DecreaseAfter || scopeChange == ScopeChange::Both) {
                context.scope--;
            }
        }

        context.depth--;
    }
}

struct DeleteEdit {
    Range range;
};

struct InsertEdit {
    Position position;
    std::string_view bytes;
};

using Edit = std::variant<DeleteEdit, InsertEdit>;

struct EditContext {
    std::vector<Edit> edits;
    
    TSPoint previousPoint;

    const Document& document;
    const Style& style;
};

void editTraverseChild(TSNode node, EditContext& context) {
    context.previousPoint = ts_node_end_point(node);
}

void HandleCompoundChild(TSNode node, uint32_t childIndex, EditContext& context, Style::BraceExistance existance, Style::BracePlacement startPlacement, Style::BracePlacement endPlacement) {
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
            
            bool startNewline = startPlacement == Style::BracePlacement::Before || startPlacement == Style::BracePlacement::Surround;
            bool innerNewline = startPlacement == Style::BracePlacement::Surround || endPlacement == Style::BracePlacement::Surround || endPlacement == Style::BracePlacement::Before;
            bool endNewline = endPlacement == Style::BracePlacement::After || endPlacement == Style::BracePlacement::Surround;

            bool insertStartNewLine = sameLinePrev && startNewline;
            bool insertInnerNewLine = sameLineInner && innerNewline;
            bool insertEndNewLine = sameLineNext && endNewline;

            if (insertStartNewLine) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(prev), .bytes = "\n"sv});
            }
            if (insertInnerNewLine) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(openBraceNode), .bytes = "\n"sv});
            }
            if (insertEndNewLine) {
                context.edits.push_back(InsertEdit{.position = Position::EndOf(closeBraceNode), .bytes = "\n"sv});
            }            
        } else if (compoundChildCount == 3 && existance == Style::BraceExistance::Remove) {
            // delete the open and close braces because there is only one statement and the style says to remove braces if there is only one statement
            
            TSNode startBrace = ts_node_child(child, 0);
            context.edits.push_back(DeleteEdit {.range = Range::Of(startBrace)});

            TSNode endBrace = ts_node_child(child, 2);
            context.edits.push_back(DeleteEdit {.range = Range::Of(endBrace)});
        }
        // else {
        //     TSNode openBraceNode = ts_node_child(child, 0);
        //     TSNode openNextNode = ts_node_child(child, 1);
        //     TSNode closePrevNode = ts_node_child(child, compoundChildCount - 2);
        //     TSNode closeBraceNode = ts_node_child(child, compoundChildCount - 1);
        //     TSNode prev = ts_node_child(node, childIndex - 1);
        //     TSNode next = ts_node_child(node, childIndex + 1);

        //     if (ts_node_is_null(next)) {
        //         next = ts_node_next_sibling(node);
        //     } 

        //     TSPoint openPoint = ts_node_start_point(openBraceNode);
        //     TSPoint closePoint = ts_node_start_point(closeBraceNode);
        //     TSPoint prevPoint = ts_node_end_point(prev);
        //     TSPoint nextPoint = ts_node_is_null(next) ? closePoint : ts_node_start_point(next);

        //     bool sameLinePrevOpen = openPoint.row == prevPoint.row;
        //     bool sameLineNextClose = closePoint.row == nextPoint.row;
        // }
    } else if (existance == Style::BraceExistance::Require) {
        // Insert open and close braces because the style requires they exist but this block doesn't have them

        TSNode prev = ts_node_child(node, childIndex - 1);

        TSPoint prevPoint = ts_node_end_point(prev);
        TSPoint point = ts_node_start_point(child);

        bool sameLinePrev = prevPoint.row == point.row;

        std::string_view openingBrace;
        switch(startPlacement) {
            case Style::BracePlacement::Before: openingBrace = "\n{"sv; break;
            case Style::BracePlacement::After: {
                openingBrace = sameLinePrev ? "{\n"sv : "{"sv;
                break;
            }
            case Style::BracePlacement::Surround: {
                openingBrace = sameLinePrev ? "\n{\n"sv : "\n{"sv;
                break;
            }
            case Style::BracePlacement::Ignore: openingBrace = "{"sv; break;
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
        switch(endPlacement) {
            case Style::BracePlacement::Before: closingBrace = "\n}"sv; break;
            case Style::BracePlacement::After: {
                closingBrace = sameLineNext ? "}\n"sv : "}"sv;
                break;
            }
            case Style::BracePlacement::Surround: {
                closingBrace = sameLineNext ? "\n}\n"sv : "\n}"sv;
                break;
            }
            case Style::BracePlacement::Ignore: closingBrace = "}"sv; break;
            default: closingBrace = "}"sv; break;                
        }

        context.edits.push_back(InsertEdit{.position = Position::EndOf(node), .bytes = closingBrace});
    }
}

void IfStatementEdits(TSNode node, uint32_t childIndex, EditContext& context) {
    std::string_view fieldName = ChildFieldName(node, childIndex);

    if (fieldName != "consequence" && fieldName != "alternative") {
        return;
    }

    HandleCompoundChild(node, childIndex, context, 
                        context.style.braceExistance.ifStatements,
                        context.style.bracePlacement.ifStatements.openingBrace,
                        context.style.bracePlacement.ifStatements.closingBrace);
}

void editsForChild(TSNode node, uint32_t childIndex, EditContext& context) {
    TSSymbol symbol = ts_node_symbol(node);

    if (symbol == IF_STATEMENT) {
         IfStatementEdits(node, childIndex, context);
    } else if (symbol == WHILE_LOOP) {
        // WhileLoopScopeChange(node, childIndex, context);
    } else if (symbol == DO_WHILE_LOOP) {
        // DoWhileLoopScopeChange(node, childIndex, context);
    } else if (symbol == COMPOUND_STATEMENT) {
        // CompoundStatementScopeChange(node, parent, childIndex, context);
    }
}

void editTraverse(TSNode node, EditContext& context) {
    if (ts_node_is_null(node)) {
        return;
    }

    uint32_t childCount = ts_node_child_count(node);
    if (childCount == 0) {
        editTraverseChild(node, context);
    } else {
        for(uint32_t i = 0; i < childCount; i++) {
            editsForChild(node, i, context);
            editTraverse(ts_node_child(node, i), context);
        }
    }
}

int main() {
    using namespace tree_sitter_format;
    Style style;

    style.indentation.indentationAmount = 1;
    style.indentation.whitespace = Style::Whitespace::Tabs;
    style.braceExistance.ifStatements = Style::BraceExistance::Require;

    Document document(inputFileName);
    std::cout << "Input Text: " << document << std::endl;

    std::string text = document.originalContents();

    const TSLanguage* cpp = tree_sitter_cpp();

    IF_STATEMENT = ts_language_symbol_for_name(cpp, "if_statement", 12, true);
    FOR_LOOP = ts_language_symbol_for_name(cpp, "for_statement", 13, true);
    FOR_RANGE_LOOP = ts_language_symbol_for_name(cpp, "for_range_loop", 14, true);
    WHILE_LOOP = ts_language_symbol_for_name(cpp, "while_statement", 15, true);
    DO_WHILE_LOOP = ts_language_symbol_for_name(cpp, "do_statement", 12, true);
    FUNCTION_DEFINITION = ts_language_symbol_for_name(cpp, "function_definition", 19, true);

    COMPOUND_STATEMENT = ts_language_symbol_for_name(cpp, "compound_statement", 18, true);
    DECLARATION_LIST = ts_language_symbol_for_name(cpp, "declaration_list", 16, true);
    ENUMERATOR_LIST = ts_language_symbol_for_name(cpp, "enumerator_list", 15, true);
    FIELD_DECLARATION_LIST  = ts_language_symbol_for_name(cpp, "field_declaration_list", 22, true);
    INITIALIZER_LIST  = ts_language_symbol_for_name(cpp, "initializer_list", 16, true);

    ERROR = ts_language_symbol_for_name(cpp, "ERROR", 5, true);

    TSNode root = document.root();

    if (ts_node_has_error(root)) {
        std::cerr << "There was an error parsing. Skipping formatting." << std::endl;
        // TODO: print the location of the parse error - traverse until an ERROR node, print line:column
        return EXIT_FAILURE;
    }


    std::ofstream output(outputFileName, std::ofstream::out | std::ofstream::trunc);
    if (!output) {
        std::cerr << "FAIL" << std::endl;
        return EXIT_FAILURE;
    }

    //std::cout << "Input Text:" << std::endl << text << std::endl;

    EditContext editContext {
        .previousPoint = ts_node_start_point(root),
        .document = document,
        .style = style,
    };

    editTraverse(root, editContext);

    size_t editCount = editContext.edits.size();
    for(size_t index = 0; index < editCount; index++) {
        Edit& edit = editContext.edits[editCount - index - 1];

        if (std::holds_alternative<DeleteEdit>(edit)) {
            DeleteEdit d = std::get<DeleteEdit>(edit);
            document.deleteBytes(d.range);
        } else if (std::holds_alternative<InsertEdit>(edit)) {
            InsertEdit i = std::get<InsertEdit>(edit);
            document.insertBytes(i.position, i.bytes);
        }
    }

    std::cout << std::endl << std::endl << "Edited:" << std::endl << document << std::endl;

    root = document.root();

    ScopeContext context {
        .depth = 0,
        .scope = 0,
        .output = output,
        .previousPosition = Position::StartOf(root),
        .document = document,
        .style = style,
    };

    std::cout << std::endl << std::endl;

    traverse(root, ts_node_parent(root), context);

    output.flush();
    output.close();


    return EXIT_SUCCESS;
}