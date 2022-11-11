#include <catch2/catch_test_macros.hpp>

#include <tree-sitter-format/Formatter.h>
#include <tree-sitter-format/traversers/CommentReflowTraverser.h>

using namespace tree_sitter_format;

std::string Reflow(const std::string& input, const Style& style) {
    Formatter formatter;
    formatter.addTraverser(std::make_unique<CommentReflowTraverser>());
    
    Document document(input);
    formatter.format(style, document);

    return document.toString();
}

TEST_CASE("Short Single Line") {
    std::string input = R"(
/* short line */
)";

    std::string expected = 
R"(
/* short line */
)";
    Style style;
    style.targetLineLength = 25;

    std::string actual = Reflow(input, style);
    REQUIRE(actual == expected);
}

TEST_CASE("Long Single Line") {
    std::string input = R"(
/* this is a really really really really really really really really really really really really really really really really really really really really really really really really long line */
)";

    std::string expected = 
R"(
/* this is a really
 * really really really
 * really really really
 * really really really
 * really really really
 * really really really
 * really really really
 * really really really
 * really really long
 * line
 */
)";
    Style style;
    style.targetLineLength = 25;

    std::string actual = Reflow(input, style);
    REQUIRE(actual == expected);
}


TEST_CASE("Long Single Line Big Words") {
    std::string input = R"(
/* this is a reallyreallyreallyreallyreallyreally reallyreallyreallyreally reallyreallyreallyreallyreallyreallyreally reallyreally reallyreally really really reallylong line */
)";

    std::string expected = 
R"(
/* this is a
 * reallyreallyreallyreallyreallyreally
 * reallyreallyreallyreally
 * reallyreallyreallyreallyreallyreallyreally
 * reallyreally
 * reallyreally really
 * really reallylong line
 */
)";
    Style style;
    style.targetLineLength = 25;

    std::string actual = Reflow(input, style);
    REQUIRE(actual == expected);
}



TEST_CASE("List Items") {
    std::string input = R"(
/* 
    1. item 2
    2. item4
      3. ifjsd
    4. asopd ijsodifjos idjfosi djofij osi
    100. aosidjf asodij
         5. oasid j asd  fewfwfwq qwd qdgg

                ajsoidj
    66. oasid j asd  fewfwfwq qwd qdgg
      ajsoidj
    7. oasid j asd  fewfwfwq qwd qdgg
       ajsoidj
    8. oasid j asd  fewfwfwq qwd qdgg
           ajsoidj
*/
)";

    std::string expected = 
R"(
/*
 *   1. item 2
 *   2. item4
 *   3. ifjsd
 *   4. asopd ijsodifjos
 *      idjfosi djofij
 *      osi 100. aosidjf
 *      asodij
 *   5. oasid j asd
 *      fewfwfwq qwd qdgg
 *
 *                 ajsoidj
 *   66. oasid j asd
 *       fewfwfwq qwd
 *       qdgg ajsoidj
 *   7. oasid j asd
 *      fewfwfwq qwd qdgg
 *      ajsoidj
 *   8. oasid j asd
 *      fewfwfwq qwd qdgg
 *            ajsoidj
 */
)";
    Style style;
    style.targetLineLength = 25;

    std::string actual = Reflow(input, style);
    REQUIRE(actual == expected);
}
