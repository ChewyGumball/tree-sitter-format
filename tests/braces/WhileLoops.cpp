#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

const std::string WHILE_NO_BRACES = R"(
while (true)
    return false;
)";

const std::string WHILE_WITH_BRACES = R"(
while (true)
    {return false;}
)";

const std::string WHILE_WITH_NO_BRACES = R"(
while (true)
    return false;
    return true;
)";

const std::string WHILE_MULTI_WITH_BRACES = R"(
while (true)
    {return false;}
    return true;
)";



std::vector<TestSuite<Style::BraceExistance>> TEST_CASES = {
    {Style::BraceExistance::Ignore, {
            {"While No Braces",         WHILE_NO_BRACES,         WHILE_NO_BRACES},
            {"While With Braces",       WHILE_WITH_BRACES,       WHILE_WITH_BRACES},
            {"While Multi No Braces",   WHILE_WITH_NO_BRACES,    WHILE_WITH_NO_BRACES},
            {"While Multi With Braces", WHILE_MULTI_WITH_BRACES, WHILE_MULTI_WITH_BRACES},
        }
    },
    {Style::BraceExistance::Remove, {
            {"While No Braces",         WHILE_NO_BRACES,         WHILE_NO_BRACES},
            {"While With Braces",       WHILE_WITH_BRACES,       WHILE_NO_BRACES},
            {"While Multi No Braces",   WHILE_WITH_NO_BRACES,    WHILE_WITH_NO_BRACES},
            {"While Multi With Braces", WHILE_MULTI_WITH_BRACES, WHILE_WITH_NO_BRACES},
        }
    },
    {Style::BraceExistance::Require, {
            {"While No Braces",         WHILE_NO_BRACES,         WHILE_WITH_BRACES},
            {"While With Braces",       WHILE_WITH_BRACES,       WHILE_WITH_BRACES},
            {"While Multi No Braces",   WHILE_WITH_NO_BRACES,    WHILE_MULTI_WITH_BRACES},
            {"While Multi With Braces", WHILE_MULTI_WITH_BRACES, WHILE_MULTI_WITH_BRACES},
        }
    },
};

TEST_CASE("While Loops") {
    TestContext<Style::BraceExistance> context {
        .styleNames = BRACE_EXISTANCE_NAMES,
        .styleUpdater = [](Style& style, Style::BraceExistance existance) {
            style.braces.whileLoops = existance;
        }
    };

    context.formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    RunTest(context, TEST_CASES);
}
