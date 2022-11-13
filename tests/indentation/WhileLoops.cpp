#include <catch2/catch_test_macros.hpp>

#include <tree-sitter-format/style/Style.h>
#include <tree-sitter-format/traversers/IndentationTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

const std::string NONE = R"(
while(false)
{
int a;
}
)";

const std::string BRACES_INDENTED = R"(
while(false)
    {
    int a;
    }
)";

const std::string BODY_INDENTED = R"(
while(false)
{
    int a;
}
)";

const std::string BOTH_INDENTED = R"(
while(false)
    {
        int a;
    }
)";

const std::string NONE_START_SAME_LINE = R"(
while(false) {
int a;
}
)";

const std::string BRACES_INDENTED_START_SAME_LINE = R"(
while(false) {
    int a;
    }
)";

const std::string BODY_INDENTED_START_SAME_LINE = R"(
while(false) {
    int a;
}
)";

const std::string BOTH_INDENTED_START_SAME_LINE = R"(
while(false) {
        int a;
    }
)";

const std::string NONE_END_SAME_LINE = R"(
while(false)
{
int a;}
)";

const std::string BRACES_INDENTED_END_SAME_LINE = R"(
while(false)
    {
    int a;}
)";

const std::string BODY_INDENTED_END_SAME_LINE = R"(
while(false)
{
    int a;}
)";

const std::string BOTH_INDENTED_END_SAME_LINE = R"(
while(false)
    {
        int a;}
)";

const std::string BOTH_SAME_LINE = R"(
while(false) {int a;}
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


TEST_CASE("While Loops") {
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.whileLoops = indentation;
        }
    };

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    RunTest(context, TEST_CASES);
}

const std::string NONE_UNFORMATTABLE_START = R"(
// tree-sitter-format off
while(false)
// tree-sitter-format on
{
int a;
}
)";

const std::string NONE_UNFORMATTABLE_START_OPEN = R"(
// tree-sitter-format off
while(false)
{
// tree-sitter-format on
int a;
}
)";

const std::string NONE_UNFORMATTABLE_START_OPEN_BODY = R"(
// tree-sitter-format off
while(false)
{
int a;
// tree-sitter-format on
}
)";

const std::string NONE_UNFORMATTABLE_START_OPEN_BODY_CLOSE = R"(
// tree-sitter-format off
while(false)
{
int a;
}
// tree-sitter-format on
)";

const std::string NONE_UNFORMATTABLE_OPEN = R"(
while(false)
// tree-sitter-format off
{
// tree-sitter-format on
int a;
}
)";

const std::string NONE_UNFORMATTABLE_OPEN_BODY = R"(
while(false)
// tree-sitter-format off
{
int a;
// tree-sitter-format on
}
)";

const std::string NONE_UNFORMATTABLE_OPEN_BODY_CLOSE = R"(
while(false)
// tree-sitter-format off
{
int a;
}
// tree-sitter-format on
)";

const std::string NONE_UNFORMATTABLE_BODY = R"(
while(false)
{
// tree-sitter-format off
int a;
// tree-sitter-format on
}
)";

const std::string NONE_UNFORMATTABLE_BODY_CLOSE = R"(
while(false)
{
// tree-sitter-format off
int a;
}
// tree-sitter-format on
)";

const std::string NONE_UNFORMATTABLE_CLOSE = R"(
while(false)
{
int a;
// tree-sitter-format off
}
// tree-sitter-format on
)";

const std::string NONE_UNFORMATTABLE_OPEN_CLOSE = R"(
while(false)
// tree-sitter-format off
{
// tree-sitter-format on
int a;
// tree-sitter-format off
}
// tree-sitter-format on
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_START = R"(
// tree-sitter-format off
while(false)
// tree-sitter-format on
    {
    int a;
    }
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_START_OPEN = R"(
// tree-sitter-format off
while(false)
{
// tree-sitter-format on
    int a;
    }
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_START_OPEN_BODY = R"(
// tree-sitter-format off
while(false)
{
int a;
// tree-sitter-format on
    }
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_START_OPEN_BODY_CLOSE = R"(
// tree-sitter-format off
while(false)
{
int a;
}
// tree-sitter-format on
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_OPEN = R"(
while(false)
// tree-sitter-format off
{
// tree-sitter-format on
    int a;
    }
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_OPEN_BODY = R"(
while(false)
// tree-sitter-format off
{
int a;
// tree-sitter-format on
    }
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_OPEN_BODY_CLOSE = R"(
while(false)
// tree-sitter-format off
{
int a;
}
// tree-sitter-format on
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_BODY = R"(
while(false)
    {
// tree-sitter-format off
int a;
// tree-sitter-format on
    }
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_BODY_CLOSE = R"(
while(false)
    {
// tree-sitter-format off
int a;
}
// tree-sitter-format on
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_CLOSE = R"(
while(false)
    {
    int a;
// tree-sitter-format off
}
// tree-sitter-format on
)";

const std::string BRACES_INDENTED_UNFORMATTABLE_OPEN_CLOSE = R"(
while(false)
// tree-sitter-format off
{
// tree-sitter-format on
    int a;
// tree-sitter-format off
}
// tree-sitter-format on
)";

// We are only going to tests unformattable regions with one style because the code
// that determines unformattablity is the same for all styles. Doing full coverage
// of styles would be an insane number of combinations.
std::vector<TestSuite<Style::Indentation>> TEST_CASES_WITH_UNFORMATTABLE_RANGES {
    {Style::Indentation::BracesIndented, {
            {"None Unformattable Start",                 NONE_UNFORMATTABLE_START,                 BRACES_INDENTED_UNFORMATTABLE_START},
            // {"None Unformattable Start Open",            NONE_UNFORMATTABLE_START_OPEN,            BRACES_INDENTED_UNFORMATTABLE_START_OPEN},
            // {"None Unformattable Start Open Body",       NONE_UNFORMATTABLE_START_OPEN_BODY,       BRACES_INDENTED_UNFORMATTABLE_START_OPEN_BODY},
            // {"None Unformattable Start Open Body Close", NONE_UNFORMATTABLE_START_OPEN_BODY_CLOSE, BRACES_INDENTED_UNFORMATTABLE_START_OPEN_BODY_CLOSE},
            // {"None Unformattable Open",                  NONE_UNFORMATTABLE_OPEN,                  BRACES_INDENTED_UNFORMATTABLE_OPEN},
            // {"None Unformattable Open Body",             NONE_UNFORMATTABLE_OPEN_BODY,             BRACES_INDENTED_UNFORMATTABLE_OPEN_BODY},
            // {"None Unformattable Open Body Close",       NONE_UNFORMATTABLE_OPEN_BODY_CLOSE,       BRACES_INDENTED_UNFORMATTABLE_OPEN_BODY_CLOSE},
            // {"None Unformattable Body Close",            NONE_UNFORMATTABLE_BODY_CLOSE,            BRACES_INDENTED_UNFORMATTABLE_BODY_CLOSE},
            // {"None Unformattable Close",                 NONE_UNFORMATTABLE_CLOSE,                 BRACES_INDENTED_UNFORMATTABLE_CLOSE},
            // {"None Unformattable Open Close",            NONE_UNFORMATTABLE_OPEN_CLOSE,            BRACES_INDENTED_UNFORMATTABLE_OPEN_CLOSE},
        }
    },
};


TEST_CASE("While Loops With Unformattable Ranges") {
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.whileLoops = indentation;
        }
    };

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    RunTest(context, TEST_CASES_WITH_UNFORMATTABLE_RANGES);
}