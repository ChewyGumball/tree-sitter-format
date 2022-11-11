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
