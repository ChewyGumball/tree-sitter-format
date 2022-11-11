#include <catch2/catch_test_macros.hpp>

#include <tree-sitter-format/Formatter.h>
#include <tree-sitter-format/traversers/BracketExistanceTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

// This is allowed by the standard, but treesitter only parses compound statements so
// tests involving this case have been disabled.
const std::string SWITCH_NO_BRACES = R"(
switch(num)
case 1: return;
)";

const std::string SWITCH_WITH_BRACES = R"(
switch(num)
{case 1: return;}
)";

const std::string MULTI_SWITCH = R"(
switch(num) {
    case 1: return;
    case 2: return;
}
)";


std::vector<TestSuite<Style::BraceExistance>> TEST_CASES = {
    {Style::BraceExistance::Ignore, {
            //{"Switch No Braces",       SWITCH_NO_BRACES,   SWITCH_NO_BRACES},
            {"Switch Braces",          SWITCH_WITH_BRACES, SWITCH_WITH_BRACES},
            {"Switch Multi Statement", MULTI_SWITCH,       MULTI_SWITCH},
        }
    },
    {Style::BraceExistance::Remove, {
            //{"Switch No Braces",       SWITCH_NO_BRACES,   SWITCH_NO_BRACES},
            {"Switch Braces",          SWITCH_WITH_BRACES, SWITCH_NO_BRACES},
            {"Switch Multi Statement", MULTI_SWITCH,       MULTI_SWITCH},
        }
    },
    {Style::BraceExistance::Require, {
            //{"Switch No Braces",       SWITCH_NO_BRACES,   SWITCH_WITH_BRACES},
            {"Switch Braces",          SWITCH_WITH_BRACES, SWITCH_WITH_BRACES},
            {"Switch Multi Statement", MULTI_SWITCH,       MULTI_SWITCH},
        }
    },
};

TEST_CASE("Switch Statements") {
    TestContext<Style::BraceExistance> context {
        .styleNames = BRACE_EXISTANCE_NAMES,
        .styleUpdater = [](Style& style, Style::BraceExistance existance) {
            style.braces.switchStatements = existance;
        }
    };

    context.style.braces.caseStatements = Style::BraceExistance::Ignore;

    context.formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    RunTest(context, TEST_CASES);
}
