#include <tree_sitter_format/Constants.h>
#include <tree_sitter_format/style/Style.h>
#include <tree_sitter_format/document/Document.h>

#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>
#include <tree_sitter_format/traversers/IndentationTraverser.h>
#include <tree_sitter_format/traversers/ParseTraverser.h>
#include <tree_sitter_format/traversers/SpaceTraverser.h>

#include <tree_sitter_format/Formatter.h>

#include <tree_sitter/api.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>

using namespace std::literals::string_view_literals;

#include <cassert>


using namespace tree_sitter_format;

const std::filesystem::path inputFileName = "G:/Projects/tree-sitter-format/tree-sitter-format/traversers/BracketExistanceTraverser.cpp";
const std::filesystem::path outputFileName = "G:/Projects/tree-sitter-format/tree-sitter-format/traversers/BracketExistanceTraverser.cpp";

int main() {
    using namespace tree_sitter_format;
    Style style;

    Document document(inputFileName);
    std::cout << "Input Text: " << std::endl << document << std::endl;

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

    Formatter formatter;
    // Ensure Braces
    formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());
    // Reindent
    formatter.addTraverser(std::make_unique<IndentationTraverser>());
    // Debug Print
    // formatter.addTraverser(std::make_unique<ParseTraverser>());
    // Trailing Space
    formatter.addTraverser(std::make_unique<SpaceTraverser>());

    formatter.format(style, document);

    output << document;

    output.flush();
    output.close();


    return EXIT_SUCCESS;
}