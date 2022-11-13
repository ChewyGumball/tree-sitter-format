#include <catch2/catch_test_macros.hpp>

#include <tree-sitter-format/Formatter.h>
#include <tree-sitter-format/traversers/BracketExistanceTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

const std::string DO_WHILE_NO_BRACES = R"(
do
    return false;
while (false);
)";

const std::string DO_WHILE_WITH_BRACES = R"(
do
    {return false;}
while (false);
)";

std::vector<TestSuite<Style::BraceExistance>> TEST_CASES = {
    {Style::BraceExistance::Ignore, {
            {"Do While No Braces",         DO_WHILE_NO_BRACES,         DO_WHILE_NO_BRACES},
            {"Do While With Braces",       DO_WHILE_WITH_BRACES,       DO_WHILE_WITH_BRACES},
        }
    },
    {Style::BraceExistance::Remove, {
            {"Do While No Braces",         DO_WHILE_NO_BRACES,         DO_WHILE_NO_BRACES},
            {"Do While With Braces",       DO_WHILE_WITH_BRACES,       DO_WHILE_NO_BRACES},
        }
    },
    {Style::BraceExistance::Require, {
            {"Do While No Braces",         DO_WHILE_NO_BRACES,         DO_WHILE_WITH_BRACES},
            {"Do While With Braces",       DO_WHILE_WITH_BRACES,       DO_WHILE_WITH_BRACES},
        }
    },
};

TEST_CASE("Do While Loops") {
    TestContext<Style::BraceExistance> context {
        .styleNames = BRACE_EXISTANCE_NAMES,
        .styleUpdater = [](Style& style, Style::BraceExistance existance) {
            style.braces.doWhileLoops = existance;
        }
    };

    context.formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    RunTest(context, TEST_CASES);
}



const std::string DO_WHILE_UNFORMATTABLE_NO_BRACES_START = R"(
// tree-sitter-format off
do
// tree-sitter-format on
    return false;
while (false);
)";

const std::string DO_WHILE_UNFORMATTABLE_NO_BRACES_START_BODY = R"(
// tree-sitter-format off
do
    return false;
// tree-sitter-format on
while (false);
)";

const std::string DO_WHILE_UNFORMATTABLE_NO_BRACES_START_BODY_CLOSE = R"(
// tree-sitter-format off
do
    return false;
while (false);
// tree-sitter-format on
)";

const std::string DO_WHILE_UNFORMATTABLE_NO_BRACES_BODY = R"(
do
// tree-sitter-format off
    return false;
// tree-sitter-format on
while (false);
)";

const std::string DO_WHILE_UNFORMATTABLE_NO_BRACES_BODY_CLOSE = R"(
do
// tree-sitter-format off
    return false;
while (false);
// tree-sitter-format on
)";

const std::string DO_WHILE_UNFORMATTABLE_NO_BRACES_CLOSE = R"(
do
    return false;
// tree-sitter-format off
while (false);
// tree-sitter-format on
)";

const std::string DO_WHILE_UNFORMATTABLE_WITH_BRACES_START = R"(
// tree-sitter-format off
do
// tree-sitter-format on
    {return false;}
while (false);
)";

const std::string DO_WHILE_UNFORMATTABLE_WITH_BRACES_START_BODY = R"(
// tree-sitter-format off
do
    return false;
// tree-sitter-format on
while (false);
)";

const std::string DO_WHILE_UNFORMATTABLE_WITH_BRACES_START_BODY_CLOSE = R"(
// tree-sitter-format off
do
    return false;
while (false);
// tree-sitter-format on
)";

const std::string DO_WHILE_UNFORMATTABLE_WITH_BRACES_BODY = R"(
do
// tree-sitter-format off
    return false;
// tree-sitter-format on
while (false);
)";

const std::string DO_WHILE_UNFORMATTABLE_WITH_BRACES_BODY_CLOSE = R"(
do
// tree-sitter-format off
    return false;
while (false);
// tree-sitter-format on
)";

const std::string DO_WHILE_UNFORMATTABLE_WITH_BRACES_CLOSE = R"(
do
    {return false;}
// tree-sitter-format off
while (false);
// tree-sitter-format on
)";



std::vector<TestSuite<Style::BraceExistance>> TEST_CASES_WITH_UNFORMATTABLE_RANGES = {
    {Style::BraceExistance::Require, {
            {"Do While No Braces Start",            DO_WHILE_UNFORMATTABLE_NO_BRACES_START,            DO_WHILE_UNFORMATTABLE_WITH_BRACES_START},
            {"Do While No Braces Start Body",       DO_WHILE_UNFORMATTABLE_NO_BRACES_START_BODY,       DO_WHILE_UNFORMATTABLE_WITH_BRACES_START_BODY},
            {"Do While No Braces Start Body Close", DO_WHILE_UNFORMATTABLE_NO_BRACES_START_BODY_CLOSE, DO_WHILE_UNFORMATTABLE_WITH_BRACES_START_BODY_CLOSE},
            {"Do While No Braces Body",             DO_WHILE_UNFORMATTABLE_NO_BRACES_BODY,             DO_WHILE_UNFORMATTABLE_WITH_BRACES_BODY},
            {"Do While No Braces Body Close",       DO_WHILE_UNFORMATTABLE_NO_BRACES_BODY_CLOSE,       DO_WHILE_UNFORMATTABLE_WITH_BRACES_BODY_CLOSE},
            {"Do While No Braces Close",            DO_WHILE_UNFORMATTABLE_NO_BRACES_CLOSE,            DO_WHILE_UNFORMATTABLE_WITH_BRACES_CLOSE},
        }
    },
};

TEST_CASE("Do While Loops With Unformattable Ranges") {
    TestContext<Style::BraceExistance> context {
        .styleNames = BRACE_EXISTANCE_NAMES,
        .styleUpdater = [](Style& style, Style::BraceExistance existance) {
            style.braces.doWhileLoops = existance;
        }
    };

    context.formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    RunTest(context, TEST_CASES_WITH_UNFORMATTABLE_RANGES);
}
