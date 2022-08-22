#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>

using namespace tree_sitter_format;

const std::string IF_NO_BRACES = R"(
if (true)
    return false;
)";

const std::string IF_WITH_BRACES = R"(
if (true)
    {return false;}
)";

const std::string MULTI_IF = R"(
if (true) {
    bool b = !a;
    return b;
}
)";

const std::string IF_NO_BRACES_ELSE_NO_BRACES = R"(
if (true)
    return false;
else
    return true;
)";

const std::string IF_NO_BRACES_ELSE_WITH_BRACES = R"(
if (true)
    return false;
else
    {return true;}
)";

const std::string IF_WITH_BRACES_ELSE_NO_BRACES = R"(
if (true)
    {return false;}
else
    return true;
)";

const std::string IF_WITH_BRACES_ELSE_WITH_BRACES = R"(
if (true)
    {return false;}
else
    {return true;}
)";

const std::string MULTI_IF_ELSE = R"(
if (true) {
    bool b = !a;
    return b;
} else {
    bool b = a;
    return b;
}
)";

TEST_CASE("If Statements") {
    Formatter formatter;
    formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    Style style;

    SECTION("Output: Require") {
        style.braces.ifStatements = Style::BraceExistance::Require;

        SECTION("Input: If No Braces") {
            Document document = Document(IF_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES);
        }

        SECTION("Input: If With Braces") {
            Document document = Document(IF_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES);
        }

        SECTION("Input: Multistatement If") {
            Document document = Document(MULTI_IF);
            formatter.format(style, document);

            REQUIRE(document.toString() == MULTI_IF);
        }

        SECTION("Input: If No Braces, Else No Braces") {
            Document document = Document(IF_NO_BRACES_ELSE_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES_ELSE_WITH_BRACES);
        }

        SECTION("Input: If No Braces, Else With Braces") {
            Document document = Document(IF_NO_BRACES_ELSE_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES_ELSE_WITH_BRACES);
        }

        SECTION("Input: If With Braces, Else No Braces") {
            Document document = Document(IF_WITH_BRACES_ELSE_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES_ELSE_WITH_BRACES);
        }

        SECTION("Input: If With Braces, Else With Braces") {
            Document document = Document(IF_WITH_BRACES_ELSE_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES_ELSE_WITH_BRACES);
        }

        SECTION("Input: Multistatement If, Multistatement Else") {
            Document document = Document(MULTI_IF_ELSE);
            formatter.format(style, document);

            REQUIRE(document.toString() == MULTI_IF_ELSE);
        }
    }

    SECTION("Output: Remove") {
        style.braces.ifStatements = Style::BraceExistance::Remove;
        
        SECTION("Input: If No Braces") {
            Document document = Document(IF_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES);
        }

        SECTION("Input: If With Braces") {
            Document document = Document(IF_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES);
        }

        SECTION("Input: Multistatement If") {
            Document document = Document(MULTI_IF);
            formatter.format(style, document);

            REQUIRE(document.toString() == MULTI_IF);
        }

        SECTION("Input: If No Braces, Else No Braces") {
            Document document = Document(IF_NO_BRACES_ELSE_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES_ELSE_NO_BRACES);
        }

        SECTION("Input: If No Braces, Else With Braces") {
            Document document = Document(IF_NO_BRACES_ELSE_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES_ELSE_NO_BRACES);
        }

        SECTION("Input: If With Braces, Else No Braces") {
            Document document = Document(IF_WITH_BRACES_ELSE_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES_ELSE_NO_BRACES);
        }

        SECTION("Input: If With Braces, Else With Braces") {
            Document document = Document(IF_WITH_BRACES_ELSE_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES_ELSE_NO_BRACES);
        }

        SECTION("Input: Multistatement If, Multistatement Else") {
            Document document = Document(MULTI_IF_ELSE);
            formatter.format(style, document);

            REQUIRE(document.toString() == MULTI_IF_ELSE);
        }
    }
    
    SECTION("Output: Ignore") {
        style.braces.ifStatements = Style::BraceExistance::Ignore;
        
        SECTION("Input: If No Braces") {
            Document document = Document(IF_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES);
        }

        SECTION("Input: If With Braces") {
            Document document = Document(IF_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES);
        }

        SECTION("Input: Multistatement If") {
            Document document = Document(MULTI_IF);
            formatter.format(style, document);

            REQUIRE(document.toString() == MULTI_IF);
        }

        SECTION("Input: If No Braces, Else No Braces") {
            Document document = Document(IF_NO_BRACES_ELSE_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES_ELSE_NO_BRACES);
        }

        SECTION("Input: If No Braces, Else With Braces") {
            Document document = Document(IF_NO_BRACES_ELSE_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_NO_BRACES_ELSE_WITH_BRACES);
        }

        SECTION("Input: If With Braces, Else No Braces") {
            Document document = Document(IF_WITH_BRACES_ELSE_NO_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES_ELSE_NO_BRACES);
        }

        SECTION("Input: If With Braces, Else With Braces") {
            Document document = Document(IF_WITH_BRACES_ELSE_WITH_BRACES);
            formatter.format(style, document);

            REQUIRE(document.toString() == IF_WITH_BRACES_ELSE_WITH_BRACES);
        }

        SECTION("Input: Multistatement If, Multistatement Else") {
            Document document = Document(MULTI_IF_ELSE);
            formatter.format(style, document);

            REQUIRE(document.toString() == MULTI_IF_ELSE);
        }
    }
}
