#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/CommentReflowTraverser.h>

using namespace tree_sitter_format;

std::string Reflow(const std::string& input, const Style& style) {
    Formatter formatter;
    formatter.addTraverser(std::make_unique<CommentReflowTraverser>());
    
    Document document(input);
    formatter.format(style, document);

    return document.toString();
}


TEST_CASE("Multiline Comments") {
    std::string input = R"(
/* this is a really really really really really really really really really really really really really really really really really really really really really really really really long line */
)";

    std::string expected = 
R"(
/* this is a really
   really really really
   really really really
   really really really
   really really really
   really really really
   really really really
   really really really
   really really long line
 */
)";
    Style style;
    style.targetLineLength = 25;

    std::string actual = Reflow(input, style);
    REQUIRE(actual == expected);
}
