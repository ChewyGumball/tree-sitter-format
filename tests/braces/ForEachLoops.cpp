#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

const std::string FOREACH_NO_BRACES = R"(
for (auto i : collection)
    return false;
)";

const std::string FOREACH_WITH_BRACES = R"(
for (auto i : collection)
    {return false;}
)";

const std::string FOREACH_WITH_NO_BRACES = R"(
for (auto i : collection)
    return false;
    return true;
)";

const std::string FOREACH_MULTI_WITH_BRACES = R"(
for (auto i : collection)
    {return false;}
    return true;
)";



std::vector<TestSuite<Style::BraceExistance>> TEST_CASES = {
    {Style::BraceExistance::Ignore, {
            {"For Each No Braces",         FOREACH_NO_BRACES,         FOREACH_NO_BRACES},
            {"For Each With Braces",       FOREACH_WITH_BRACES,       FOREACH_WITH_BRACES},
            {"For Each Multi No Braces",   FOREACH_WITH_NO_BRACES,    FOREACH_WITH_NO_BRACES},
            {"For Each Multi With Braces", FOREACH_MULTI_WITH_BRACES, FOREACH_MULTI_WITH_BRACES},
        }
    },
    {Style::BraceExistance::Remove, {
            {"For Each No Braces",         FOREACH_NO_BRACES,         FOREACH_NO_BRACES},
            {"For Each With Braces",       FOREACH_WITH_BRACES,       FOREACH_NO_BRACES},
            {"For Each Multi No Braces",   FOREACH_WITH_NO_BRACES,    FOREACH_WITH_NO_BRACES},
            {"For Each Multi With Braces", FOREACH_MULTI_WITH_BRACES, FOREACH_WITH_NO_BRACES},
        }
    },
    {Style::BraceExistance::Require, {
            {"For Each No Braces",         FOREACH_NO_BRACES,         FOREACH_WITH_BRACES},
            {"For Each With Braces",       FOREACH_WITH_BRACES,       FOREACH_WITH_BRACES},
            {"For Each Multi No Braces",   FOREACH_WITH_NO_BRACES,    FOREACH_MULTI_WITH_BRACES},
            {"For Each Multi With Braces", FOREACH_MULTI_WITH_BRACES, FOREACH_MULTI_WITH_BRACES},
        }
    },
};

TEST_CASE("For Each Loops") {
    TestContext<Style::BraceExistance> context {
        .styleNames = BRACE_EXISTANCE_NAMES,
        .styleUpdater = [](Style& style, Style::BraceExistance existance) {
            style.braces.forLoops = existance;
        }
    };

    context.formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    RunTest(context, TEST_CASES);
}
