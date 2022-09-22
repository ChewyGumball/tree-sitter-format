#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/style/Style.h>
#include <tree_sitter_format/traversers/IndentationTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

const std::string NONE = R"(
[]()
{
int a;
}
)";

const std::string BRACES_INDENTED = R"(
[]()
    {
    int a;
    }
)";

const std::string BODY_INDENTED = R"(
[]()
{
    int a;
}
)";

const std::string BOTH_INDENTED = R"(
[]()
    {
        int a;
    }
)";

const std::string NONE_START_SAME_LINE = R"(
[]() {
int a;
}
)";

const std::string BRACES_INDENTED_START_SAME_LINE = R"(
[]() {
    int a;
    }
)";

const std::string BODY_INDENTED_START_SAME_LINE = R"(
[]() {
    int a;
}
)";

const std::string BOTH_INDENTED_START_SAME_LINE = R"(
[]() {
        int a;
    }
)";

const std::string NONE_END_SAME_LINE = R"(
[]() 
{
int a;}
)";

const std::string BRACES_INDENTED_END_SAME_LINE = R"(
[]() 
    {
    int a;}
)";

const std::string BODY_INDENTED_END_SAME_LINE = R"(
[]() 
{
    int a;}
)";

const std::string BOTH_INDENTED_END_SAME_LINE = R"(
[]() 
    {
        int a;}
)";

const std::string BOTH_SAME_LINE = R"(
[]() {int a;}
)";

std::vector<TestSuite<Style::Indentation>> TEST_CASES {
    {Style::Indentation::None, {
            {"None",                            NONE,                            NONE},
            {"Braces Indented",                 BRACES_INDENTED,                 NONE},
            {"Body Indented",                   BODY_INDENTED,                   NONE},
            {"Both Indented",                   BOTH_INDENTED,                   NONE},
            {"None Start Same Line",            NONE_START_SAME_LINE,            NONE_START_SAME_LINE},
            {"Braces Indented Start Same Line", BRACES_INDENTED_START_SAME_LINE, NONE_START_SAME_LINE},
            {"Body Indented Start Same Line",   BODY_INDENTED_START_SAME_LINE,   NONE_START_SAME_LINE},
            {"Both Indented Start Same Line",   BOTH_INDENTED_START_SAME_LINE,   NONE_START_SAME_LINE},
            {"None End Same Line",              NONE_END_SAME_LINE,              NONE_END_SAME_LINE},
            {"Braces Indented End Same Line",   BRACES_INDENTED_END_SAME_LINE,   NONE_END_SAME_LINE},
            {"Body Indented End Same Line",     BODY_INDENTED_END_SAME_LINE,     NONE_END_SAME_LINE},
            {"Both Indented End Same Line",     BOTH_INDENTED_END_SAME_LINE,     NONE_END_SAME_LINE},
            {"Both Same Line",                  BOTH_SAME_LINE,                  BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BracesIndented, {
            {"None",                            NONE,                            BRACES_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BRACES_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BRACES_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BRACES_INDENTED},
            {"None Start Same Line",            NONE_START_SAME_LINE,            BRACES_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", BRACES_INDENTED_START_SAME_LINE, BRACES_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   BODY_INDENTED_START_SAME_LINE,   BRACES_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   BOTH_INDENTED_START_SAME_LINE,   BRACES_INDENTED_START_SAME_LINE},
            {"None End Same Line",              NONE_END_SAME_LINE,              BRACES_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   BRACES_INDENTED_END_SAME_LINE,   BRACES_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     BODY_INDENTED_END_SAME_LINE,     BRACES_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     BOTH_INDENTED_END_SAME_LINE,     BRACES_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  BOTH_SAME_LINE,                  BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BodyIndented, {
            {"None",                            NONE,                            BODY_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BODY_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BODY_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BODY_INDENTED},
            {"None Start Same Line",            NONE_START_SAME_LINE,            BODY_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", BRACES_INDENTED_START_SAME_LINE, BODY_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   BODY_INDENTED_START_SAME_LINE,   BODY_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   BOTH_INDENTED_START_SAME_LINE,   BODY_INDENTED_START_SAME_LINE},
            {"None End Same Line",              NONE_END_SAME_LINE,              BODY_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   BRACES_INDENTED_END_SAME_LINE,   BODY_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     BODY_INDENTED_END_SAME_LINE,     BODY_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     BOTH_INDENTED_END_SAME_LINE,     BODY_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  BOTH_SAME_LINE,                  BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BothIndented, {
            {"None",                            NONE,                            BOTH_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BOTH_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BOTH_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BOTH_INDENTED},
            {"None Start Same Line",            NONE_START_SAME_LINE,            BOTH_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", BRACES_INDENTED_START_SAME_LINE, BOTH_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   BODY_INDENTED_START_SAME_LINE,   BOTH_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   BOTH_INDENTED_START_SAME_LINE,   BOTH_INDENTED_START_SAME_LINE},
            {"None End Same Line",              NONE_END_SAME_LINE,              BOTH_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   BRACES_INDENTED_END_SAME_LINE,   BOTH_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     BODY_INDENTED_END_SAME_LINE,     BOTH_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     BOTH_INDENTED_END_SAME_LINE,     BOTH_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  BOTH_SAME_LINE,                  BOTH_SAME_LINE},
        }
    },
};


TEST_CASE("Lambdas") {    
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.lambdas = indentation;
        }
    };

    context.style.indentation.genericScope = Style::Indentation::None;

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    RunTest(context, TEST_CASES);
}
