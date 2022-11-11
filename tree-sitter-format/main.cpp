#include <tree-sitter-format/Constants.h>
#include <tree-sitter-format/style/Style.h>
#include <tree-sitter-format/document/Document.h>

#include <tree-sitter-format/traversers/BracketExistanceTraverser.h>
#include <tree-sitter-format/traversers/IndentationTraverser.h>
#include <tree-sitter-format/traversers/ParseTraverser.h>
#include <tree-sitter-format/traversers/SpaceTraverser.h>
#include <tree-sitter-format/traversers/DeclarationAlignmentTraverser.h>
#include <tree-sitter-format/traversers/BitfieldAlignmentTraverser.h>
#include <tree-sitter-format/traversers/AssignmentAlignmentTraverser.h>
#include <tree-sitter-format/traversers/InitializerListAlignmentTraverser.h>

#include <tree-sitter-format/Formatter.h>

#include <tree_sitter/api.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>

using namespace std::literals::string_view_literals;

#include <cassert>


using namespace tree_sitter_format;

const std::filesystem::path inputFileName = "G:\\Projects\\tree-sitter-format\\test.cpp";
const std::filesystem::path outputFileName = "G:\\Projects\\tree-sitter-format\\test.cpp.out";

int main() {
    #ifdef _DEBUG
    std::cout << "_DEBUG defined" << std::endl;
    #endif

    #ifdef NDEBUG
    std::cout << "NDEBUG defined" << std::endl;
    #endif

    using namespace tree_sitter_format;
    Style style;

    style.braces.forLoops = Style::BraceExistance::Require;
    style.indentation.reindent = false;

    Document document(inputFileName);
    std::cout << "Input Text: " << std::endl << std::endl;

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
    //formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());
    // Reindent
    //formatter.addTraverser(std::make_unique<IndentationTraverser>());
    // Debug Print
    formatter.addTraverser(std::make_unique<ParseTraverser>());
    // Trailing Space
    formatter.addTraverser(std::make_unique<SpaceTraverser>());
    // Alignment
    formatter.addTraverser(std::make_unique<DeclarationAlignmentTraverser>());
    formatter.addTraverser(std::make_unique<BitfieldAlignmentTraverser>());
    formatter.addTraverser(std::make_unique<AssignmentAlignmentTraverser>());
    formatter.addTraverser(std::make_unique<InitializerListAlignmentTraverser>());

    formatter.format(style, document);

    output << document;

    output.flush();
    output.close();


    return EXIT_SUCCESS;
}