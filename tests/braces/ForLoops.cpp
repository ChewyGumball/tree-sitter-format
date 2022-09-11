#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

const std::string FOR_NO_BRACES = R"(
for (int i = 0; i < 4; i++)
    return false;
)";

const std::string FOR_WITH_BRACES = R"(
for (int i = 0; i < 4; i++)
    {return false;}
)";

const std::string FOR_WITH_NO_BRACES = R"(
for (int i = 0; i < 4; i++)
    return false;
    return true;
)";

const std::string FOR_MULTI_WITH_BRACES = R"(
for (int i = 0; i < 4; i++)
    {return false;}
    return true;
)";



std::vector<TestSuite<Style::BraceExistance>> TEST_CASES = {
    {Style::BraceExistance::Ignore, {
            {"For No Braces",         FOR_NO_BRACES,         FOR_NO_BRACES},
            {"For With Braces",       FOR_WITH_BRACES,       FOR_WITH_BRACES},
            {"For Multi No Braces",   FOR_WITH_NO_BRACES,    FOR_WITH_NO_BRACES},
            {"For Multi With Braces", FOR_MULTI_WITH_BRACES, FOR_MULTI_WITH_BRACES},
        }
    },
    {Style::BraceExistance::Remove, {
            {"For No Braces",         FOR_NO_BRACES,         FOR_NO_BRACES},
            {"For With Braces",       FOR_WITH_BRACES,       FOR_NO_BRACES},
            {"For Multi No Braces",   FOR_WITH_NO_BRACES,    FOR_WITH_NO_BRACES},
            {"For Multi With Braces", FOR_MULTI_WITH_BRACES, FOR_WITH_NO_BRACES},
        }
    },
    {Style::BraceExistance::Require, {
            {"For No Braces",         FOR_NO_BRACES,         FOR_WITH_BRACES},
            {"For With Braces",       FOR_WITH_BRACES,       FOR_WITH_BRACES},
            {"For Multi No Braces",   FOR_WITH_NO_BRACES,    FOR_MULTI_WITH_BRACES},
            {"For Multi With Braces", FOR_MULTI_WITH_BRACES, FOR_MULTI_WITH_BRACES},
        }
    },
};

TEST_CASE("For Loops") {
    TestContext<Style::BraceExistance> context {
        .styleNames = BRACE_EXISTANCE_NAMES,
        .styleUpdater = [](Style& style, Style::BraceExistance existance) {
            style.braces.forLoops = existance;
        }
    };

    context.formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    RunTest(context, TEST_CASES);
}
