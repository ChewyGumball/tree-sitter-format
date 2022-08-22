#include <tree_sitter_format/Constants.h>
#include <tree_sitter_format/style/Style.h>
#include <tree_sitter_format/document/Document.h>

#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>
#include <tree_sitter_format/traversers/IndentationTraverser.h>
#include <tree_sitter_format/traversers/ParseTraverser.h>
#include <tree_sitter_format/traversers/SpaceTraverser.h>

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

int main() {
    using namespace tree_sitter_format;
    Style style;

    style.indentation.indentationAmount = 1;
    style.indentation.whitespace = Style::IndentationWhitespace::Tabs;
    style.braces.ifStatements = Style::BraceExistance::Require;

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
    BracketExistanceTraverser b(style);
    b.traverse(document.root());
    document.applyEdits(b.edits());

    // Reindent
    IndentationTraverser t(document, style);
    t.traverse(document.root());
    document.applyEdits(t.edits());

    // Debug Print
    // ParseTraverser p;
    // p.traverse(document.root());


    // Trailing Space
    SpaceTraverser s(document, style);
    s.traverse(document.root());
    document.applyEdits(s.edits());

    output << document;

    output.flush();
    output.close();


    return EXIT_SUCCESS;
}