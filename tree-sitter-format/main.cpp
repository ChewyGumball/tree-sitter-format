#include <tree_sitter_format/style/Style.h>
#include <tree_sitter_format/document/Document.h>

#include <tree_sitter/api.h>

#include <iostream>
#include <fstream>
#include <sstream>

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

struct Context {
    uint32_t depth = 0;
    uint32_t scope = 0;
    std::ostream& output;
    Document& document;

    TSPoint previousPoint;

    const Style& style;
};

void traverse(TSNode node, Context& context);

void traverseGeneric(TSNode node, Context& context) {
    uint32_t childCount = ts_node_child_count(node);
    for(uint32_t i = 0; i < childCount; i++) {
        traverse(ts_node_child(node, i), context);
    }
}

void traverseChild(TSNode node, Context& context) {
    std::string_view token = context.document.originalContentsAt(node);

    TSPoint currentPoint = ts_node_start_point(node);
    for(uint32_t i = context.previousPoint.row; i < currentPoint.row; i++) {
        context.output << std::endl;
    }

    if (context.previousPoint.row != currentPoint.row) {
        for(uint32_t i = 0; i < context.scope; i++) {
            context.output << context.style.indentationString();
        }
    } else {
        const std::string& text = context.document.originalContents();

        uint32_t startByte = ts_node_start_byte(node);;
        uint32_t whiteSpace = startByte;
        while (whiteSpace > 0 && (text[whiteSpace - 1] == ' ' || text[whiteSpace - 1] == '\t')) {
            whiteSpace--;
        }

        context.output << context.document.originalContentsAt(Document::Range(whiteSpace, startByte));
    }

    context.output << token;

    context.previousPoint = ts_node_end_point(node);
}

void traverseIfStatement(TSNode node, Context& context) {
    assert(ts_node_symbol(node) == IF_STATEMENT);

    uint32_t childCount = ts_node_child_count(node);
    for(uint32_t i = 0; i < childCount; i++) {
        TSNode child = ts_node_child(node, i);

        std::string fieldName;
        const char* fieldNameChars = ts_node_field_name_for_child(node, i);
        if (fieldNameChars != nullptr) {
            fieldName = fieldNameChars;
        }

        if (fieldName == "consequence" || fieldName == "alternative") {
            if (ts_node_symbol(child) != COMPOUND_STATEMENT) {
                context.scope++;
            }
        }

        traverse(child, context);

        if (fieldName == "consequence" || fieldName == "alternative") {
            if (ts_node_symbol(child) != COMPOUND_STATEMENT) {
                context.scope--;
            }
        }
    }
}

void traverseCompoundStatement(TSNode node, Context& context) {
    uint32_t childCount = ts_node_child_count(node);
    for(uint32_t i = 0; i < childCount; i++) {
        if (i == 1) {
            context.scope++;
        }
        // This is not an else, because of {} - when looking at the },
        // it is the child at index 1, but ALSO the last child so no
        // scope indentation change necessary.
        if (i == childCount - 1) {
            context.scope--;
        }

        traverse(ts_node_child(node, i), context);
    }
}

void traverseDeclarationList(TSNode node, Context& context) {
    traverseCompoundStatement(node, context);
}

void traverseFieldDeclarationList(TSNode node, Context& context) {
    traverseCompoundStatement(node, context);
}

void traverseEnumeratorList(TSNode node, Context& context) {
    traverseCompoundStatement(node, context);
}

void traverseInitializationList(TSNode node, Context& context) {
    traverseCompoundStatement(node, context);
}

void traverseForLoop(TSNode node, Context& context) {
    uint32_t childCount = ts_node_child_count(node);
    for(uint32_t i = 0; i < childCount; i++) {
        TSNode child = ts_node_child(node, i);

        if (i == childCount - 1 && ts_node_symbol(child) != COMPOUND_STATEMENT) {
            context.scope++;
        }

        traverse(child, context);

        if (i == childCount - 1 && ts_node_symbol(child) != COMPOUND_STATEMENT) {
            context.scope--;
        }
    }
}

void traverseForRangeLoop(TSNode node, Context& context) {
    traverseForLoop(node, context);
}

void traverseWhileLoop(TSNode node, Context& context) {
    uint32_t childCount = ts_node_child_count(node);
    for(uint32_t i = 0; i < childCount; i++) {
        TSNode child = ts_node_child(node, i);

        std::string fieldName;
        const char* fieldNameChars = ts_node_field_name_for_child(node, i);
        if (fieldNameChars != nullptr) {
            fieldName = fieldNameChars;
        }

        if (fieldName == "body") {
            if (ts_node_symbol(child) != COMPOUND_STATEMENT) {
                context.scope++;
            }
        }

        traverse(child, context);

        if (fieldName == "body") {
            if (ts_node_symbol(child) != COMPOUND_STATEMENT) {
                context.scope--;
            }
        }
    }
}

void traverseDoWhileLoop(TSNode node, Context& context) {
    traverseWhileLoop(node, context);
}

void traverse(TSNode node, Context& context) {
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

        TSSymbol symbol = ts_node_symbol(node);
        if(symbol == IF_STATEMENT) {
            traverseIfStatement(node, context);
        } else if (symbol == FOR_LOOP) {
            traverseForLoop(node, context);
        } else if (symbol == FOR_RANGE_LOOP) {
            traverseForRangeLoop(node, context);
        } else if (symbol == WHILE_LOOP) {
            traverseWhileLoop(node, context);
        } else if (symbol == DO_WHILE_LOOP) {
            traverseDoWhileLoop(node, context);
        } else if (symbol == COMPOUND_STATEMENT) {
            traverseCompoundStatement(node, context);
        } else if (symbol == DECLARATION_LIST) {
            traverseDeclarationList(node, context);
        } else if (symbol == ENUMERATOR_LIST) {
            traverseEnumeratorList(node, context);
        } else if (symbol == FIELD_DECLARATION_LIST) {
            traverseFieldDeclarationList(node, context);
        } else if (symbol == INITIALIZER_LIST) {
            traverseInitializationList(node, context);
        } else {
            traverseGeneric(node, context);
        }
        context.depth--;
    }
}

int main() {
    using namespace tree_sitter_format;
    Style style;

    style.indentation.indentationAmount = 1;
    style.indentation.whitespace = Style::Whitespace::Tabs;

    Document document(inputFileName);
    std::cout << "Input Text: " << document << std::endl;

    std::string text = document.originalContents();

    const TSLanguage* cpp = tree_sitter_cpp();
    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, cpp);

    IF_STATEMENT = ts_language_symbol_for_name(cpp, "if_statement", 12, true);
    FOR_LOOP = ts_language_symbol_for_name(cpp, "for_statement", 13, true);
    FOR_RANGE_LOOP = ts_language_symbol_for_name(cpp, "for_range_loop", 14, true);
    WHILE_LOOP = ts_language_symbol_for_name(cpp, "while_statement", 15, true);
    DO_WHILE_LOOP = ts_language_symbol_for_name(cpp, "do_statement", 12, true);

    COMPOUND_STATEMENT = ts_language_symbol_for_name(cpp, "compound_statement", 18, true);
    DECLARATION_LIST = ts_language_symbol_for_name(cpp, "declaration_list", 16, true);
    ENUMERATOR_LIST = ts_language_symbol_for_name(cpp, "enumerator_list", 15, true);
    FIELD_DECLARATION_LIST  = ts_language_symbol_for_name(cpp, "field_declaration_list", 22, true);
    INITIALIZER_LIST  = ts_language_symbol_for_name(cpp, "initializer_list", 16, true);

    ERROR = ts_language_symbol_for_name(cpp, "ERROR", 5, true);


    TSTree* tree = ts_parser_parse(parser, nullptr, document.inputReader());
    TSNode root = ts_tree_root_node(tree);

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

    Context context {
        .depth = 0,
        .scope = 0,
        .output = output,
        .document = document,
        .previousPoint = ts_node_start_point(root),
        .style = style,
    };

    std::cout << std::endl << std::endl;

    traverse(root, context);

    output.flush();
    output.close();


    return EXIT_SUCCESS;
}