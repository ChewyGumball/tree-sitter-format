#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

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

const std::string FAKE_MULTI_IF = R"(
if(true)
bool b = !a;
return b;
)";

const std::string REAL_MULTI_IF = R"(
if(true) {
bool b = !a;
}
return b;
)";



std::vector<TestSuite<Style::BraceExistance>> TEST_CASES = {
    {Style::BraceExistance::Ignore, {
            {"If No Braces",                            IF_NO_BRACES,                    IF_NO_BRACES},
            {"If Braces",                               IF_WITH_BRACES,                  IF_WITH_BRACES},
            {"If Multi Statement",                      MULTI_IF,                        MULTI_IF},
            {"If No Braces Else No Braces",             IF_NO_BRACES_ELSE_NO_BRACES,     IF_NO_BRACES_ELSE_NO_BRACES},
            {"If No Braces Else Braces",                IF_NO_BRACES_ELSE_WITH_BRACES,   IF_NO_BRACES_ELSE_WITH_BRACES},
            {"If Braces Else No Braces",                IF_WITH_BRACES_ELSE_NO_BRACES,   IF_WITH_BRACES_ELSE_NO_BRACES},
            {"If Braces Else Braces",                   IF_WITH_BRACES_ELSE_WITH_BRACES, IF_WITH_BRACES_ELSE_WITH_BRACES},
            {"If Multi Statement Else Multi Statement", MULTI_IF_ELSE,                   MULTI_IF_ELSE},
            {"If Fake Multi",                           FAKE_MULTI_IF,                   FAKE_MULTI_IF},
            {"If Real Multi",                           REAL_MULTI_IF,                   REAL_MULTI_IF},
        }
    },
    {Style::BraceExistance::Remove, {
            {"If No Braces",                            IF_NO_BRACES,                    IF_NO_BRACES},
            {"If Braces",                               IF_WITH_BRACES,                  IF_NO_BRACES},
            {"If Multi Statement",                      MULTI_IF,                        MULTI_IF},
            {"If No Braces Else No Braces",             IF_NO_BRACES_ELSE_NO_BRACES,     IF_NO_BRACES_ELSE_NO_BRACES},
            {"If No Braces Else Braces",                IF_NO_BRACES_ELSE_WITH_BRACES,   IF_NO_BRACES_ELSE_NO_BRACES},
            {"If Braces Else No Braces",                IF_WITH_BRACES_ELSE_NO_BRACES,   IF_WITH_BRACES_ELSE_NO_BRACES},
            {"If Braces Else Braces",                   IF_WITH_BRACES_ELSE_WITH_BRACES, IF_WITH_BRACES_ELSE_NO_BRACES},
            {"If Multi Statement Else Multi Statement", MULTI_IF_ELSE,                   MULTI_IF_ELSE},
            {"If Fake Multi",                           FAKE_MULTI_IF,                   FAKE_MULTI_IF},
            {"If Real Multi",                           REAL_MULTI_IF,                   FAKE_MULTI_IF},
        }
    },
    {Style::BraceExistance::Require, {
            {"If No Braces",                            IF_NO_BRACES,                    IF_WITH_BRACES},
            {"If Braces",                               IF_WITH_BRACES,                  IF_WITH_BRACES},
            {"If Multi Statement",                      MULTI_IF,                        MULTI_IF},
            {"If No Braces Else No Braces",             IF_NO_BRACES_ELSE_NO_BRACES,     IF_NO_BRACES_ELSE_WITH_BRACES},
            {"If No Braces Else Braces",                IF_NO_BRACES_ELSE_WITH_BRACES,   IF_NO_BRACES_ELSE_WITH_BRACES},
            {"If Braces Else No Braces",                IF_WITH_BRACES_ELSE_NO_BRACES,   IF_WITH_BRACES_ELSE_WITH_BRACES},
            {"If Braces Else Braces",                   IF_WITH_BRACES_ELSE_WITH_BRACES, IF_WITH_BRACES_ELSE_WITH_BRACES},
            {"If Multi Statement Else Multi Statement", MULTI_IF_ELSE,                   MULTI_IF_ELSE},
            {"If Fake Multi",                           FAKE_MULTI_IF,                   REAL_MULTI_IF},
            {"If Real Multi",                           REAL_MULTI_IF,                   REAL_MULTI_IF},
        }
    },
};

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
