#include <tree_sitter_format/style/Style.h>
#include <tree_sitter_format/document/Document.h>

#include <tree_sitter/api.h>

#include <iostream>
#include <fstream>
#include <sstream>

extern "C" {
const TSLanguage* tree_sitter_cpp(void);
}

const std::filesystem::path inputFileName = "C:/Users/bpdun/Desktop/test/test.cpp";
const std::filesystem::path outputFileName = "C:/Users/bpdun/Desktop/test/test.cpp.out";

std::string_view ToStr(TSNode node, std::string_view text) {
    if (ts_node_is_null(node)) {
        return "(null)";
    }

    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);

    return text.substr(start, end - start);
}

int main() {
    using namespace tree_sitter_format;
    Style style;

    style.indentation.indentationAmount = 1;
    style.indentation.whitespace = Style::Whitespace::Tabs;

    Document document(inputFileName);
    std::cout << "Input Text: " << document << std::endl;

    std::string text = document.originalContents();

    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree* tree = ts_parser_parse(parser, nullptr, document.inputReader());
    TSNode root = ts_tree_root_node(tree);

    std::ofstream output(outputFileName, std::ofstream::out | std::ofstream::trunc);
    if (!output) {
        std::cout << "FAIL" << std::endl;
        return EXIT_FAILURE;
    }

    //std::cout << "Input Text:" << std::endl << text << std::endl;

    TSNode node = root;
    TSPoint previousPoint = ts_node_start_point(root);

    uint32_t scopeCount = 0;
    while(!ts_node_is_null(node)) {
        uint32_t childCount = ts_node_child_count(node);
        if (childCount == 0) {

            std::string_view token = document.originalContentsAt(node);
            if (token == "{") {
                scopeCount++;
            } else if (token == "}") {
                scopeCount--;
            }

            TSPoint currentPoint = ts_node_start_point(node);
            for(uint32_t i = previousPoint.row; i < currentPoint.row; i++) {
                output << std::endl;
            }
            
            if (previousPoint.row != currentPoint.row) {
                
                for(uint32_t i = 0; i < scopeCount; i++) {
                    output << style.indentationString();
                }
            } else {
                uint32_t startByte = ts_node_start_byte(node);;
                uint32_t whiteSpace = startByte;
                while (whiteSpace > 0 && (text[whiteSpace - 1] == ' ' || text[whiteSpace - 1] == '\t')) {
                    whiteSpace--;
                }

                output << document.originalContentsAt(Document::Range(whiteSpace, startByte));
            }
            
            output << token;

            previousPoint = ts_node_end_point(node);

            TSNode next = ts_node_next_sibling(node);
            if (ts_node_is_null(next)) {
                do {
                    node = ts_node_parent(node);
                    next = ts_node_next_sibling(node);
                } while(ts_node_is_null(next) && !ts_node_eq(node, root));

            }

            node = next;
        } else {
            node = ts_node_child(node, 0);
        }
    }

    output.flush();
    output.close();


    return EXIT_SUCCESS;
}