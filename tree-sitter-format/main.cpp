#include <tree_sitter_format/Constants.h>
#include <tree_sitter_format/style/Style.h>
#include <tree_sitter_format/document/Document.h>

#include <tree_sitter_format/traversers/IndentationTraverser.h>
#include <tree_sitter_format/traversers/Traverser.h>

#include <tree_sitter/api.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>

using namespace std::literals::string_view_literals;

#include <cassert>


using namespace tree_sitter_format;

const std::filesystem::path inputFileName = "C:/Users/bpdun/Desktop/test/test.cpp";
const std::filesystem::path outputFileName = "C:/Users/bpdun/Desktop/test/test.cpp.out";


std::string_view ChildFieldName(TSNode node, uint32_t childIndex) {
    const char* name = ts_node_field_name_for_child(node, childIndex);
    if(name == nullptr) {
        return std::string_view();
    } else {
        return name;
    }
}

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
    std::cout << "Input Text: " << std::endl << document << std::endl;

    std::string text = document.originalContents();

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

    // Ensure Braces

    EditContext editContext {
        .previousPoint = ts_node_start_point(root),
        .document = document,
        .style = style,
    };

    editTraverse(root, editContext);
    document.applyEdits(editContext.edits);

    // Reindent

    root = document.root();

    IndentationContext context {
        .scope = 0,
        .previousPosition = Position::StartOf(root),
        .style = style,
    };

    IndentationTraverser t(style);
    t.traverse(root);
    document.applyEdits(t.edits());

    output << document;

    output.flush();
    output.close();


    return EXIT_SUCCESS;
}