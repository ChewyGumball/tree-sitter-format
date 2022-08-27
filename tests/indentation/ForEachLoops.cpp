#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/IndentationTraverser.h>

using namespace tree_sitter_format;

const std::string NONE = R"(
for(auto i : thing)
{
int a;
}
)";

const std::string BRACES_INDENTED = R"(
for(auto i : thing)
    {
    int a;
    }
)";

const std::string BODY_INDENTED = R"(
for(auto i : thing)
{
    int a;
}
)";

const std::string BOTH_INDENTED = R"(
for(auto i : thing)
    {
        int a;
    }
)";


TEST_CASE("For Each Loops") {
    Formatter formatter;
    formatter.addTraverser(std::make_unique<IndentationTraverser>());

    Style style;
    style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    SECTION("Output: None") {
        style.indentation.forLoops = Style::Indentation::None;

        SECTION("Input: None") {
            Document document(NONE);
            formatter.format(style, document);

            REQUIRE(document.toString() == NONE);
        }
        
        SECTION("Input: Braces Indented") {
            Document document(BRACES_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == NONE);
        }
        
        SECTION("Input: Body Indented") {
            Document document(BODY_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == NONE);
        }
        
        SECTION("Input: Both Indented") {
            Document document(BOTH_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == NONE);
        }
    }

    SECTION("Output: Braces Indented") {
        style.indentation.forLoops = Style::Indentation::BracesIndented;

        SECTION("Input: None") {
            Document document(NONE);
            formatter.format(style, document);

            REQUIRE(document.toString() == BRACES_INDENTED);
        }
        
        SECTION("Input: Braces Indented") {
            Document document(BRACES_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BRACES_INDENTED);
        }
        
        SECTION("Input: Body Indented") {
            Document document(BODY_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BRACES_INDENTED);
        }
        
        SECTION("Input: Both Indented") {
            Document document(BOTH_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BRACES_INDENTED);
        }
    }
    
    SECTION("Output: Body Indented") {
        style.indentation.forLoops = Style::Indentation::BodyIndented;

        SECTION("Input: None") {
            Document document(NONE);
            formatter.format(style, document);

            REQUIRE(document.toString() == BODY_INDENTED);
        }
        
        SECTION("Input: Braces Indented") {
            Document document(BRACES_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BODY_INDENTED);
        }
        
        SECTION("Input: Body Indented") {
            Document document(BODY_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BODY_INDENTED);
        }
        
        SECTION("Input: Both Indented") {
            Document document(BOTH_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BODY_INDENTED);
        }
    }
    
    SECTION("Output: Both Indented") {
        style.indentation.forLoops = Style::Indentation::BothIndented;

        SECTION("Input: None") {
            Document document(NONE);
            formatter.format(style, document);

            REQUIRE(document.toString() == BOTH_INDENTED);
        }
        
        SECTION("Input: Braces Indented") {
            Document document(BRACES_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BOTH_INDENTED);
        }
        
        SECTION("Input: Body Indented") {
            Document document(BODY_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BOTH_INDENTED);
        }
        
        SECTION("Input: Both Indented") {
            Document document(BOTH_INDENTED);
            formatter.format(style, document);

            REQUIRE(document.toString() == BOTH_INDENTED);
        }
    }
}
