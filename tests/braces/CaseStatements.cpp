#include <catch2/catch_test_macros.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/BracketExistanceTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

// This is allowed by the standard, but treesitter only parses compound statements so
// tests involving this case have been disabled.
const std::string CASE_NO_BRACES = R"(
switch(num) {
case 1: return;
}
)";

const std::string CASE_NO_BRACES_MULTI_STATEMENT = R"(
switch(num) {
case 1:
int i = 3
return;
}
)";

const std::string CASE_WITH_BRACES = R"(
switch(num) {
case 1: {return;}
}
)";

const std::string CASE_WITH_BRACES_MULTI_STATEMENT = R"(
switch(num) {
case 1:
{int i = 3
return;}
}
)";

const std::string MULTI_CASE_NO_BRACES = R"(
switch(num) {
    case 1: return;
    case 2: return;
    default: return;
}
)";

const std::string MULTI_CASE_WITH_BRACES = R"(
switch(num) {
    case 1: {return;}
    case 2: {return;}
    default: {return;}
}
)";


std::vector<TestSuite<Style::BraceExistance>> TEST_CASES = {
    {Style::BraceExistance::Ignore, {
            {"Case No Braces",                 CASE_NO_BRACES,                   CASE_NO_BRACES},
            {"Case No Braces Multi Statement", CASE_NO_BRACES_MULTI_STATEMENT,   CASE_NO_BRACES_MULTI_STATEMENT},
            {"Case Braces",                    CASE_WITH_BRACES,                 CASE_WITH_BRACES},
            {"Case Braces Multi Statement",    CASE_WITH_BRACES_MULTI_STATEMENT, CASE_WITH_BRACES_MULTI_STATEMENT},
            {"Multi Case No Braces",           MULTI_CASE_NO_BRACES,             MULTI_CASE_NO_BRACES},
            {"Multi Case With Braces",         MULTI_CASE_WITH_BRACES,           MULTI_CASE_WITH_BRACES},
        }
    },
    {Style::BraceExistance::Remove, {
            {"Case No Braces",                 CASE_NO_BRACES,                   CASE_NO_BRACES},
            {"Case No Braces Multi Statement", CASE_NO_BRACES_MULTI_STATEMENT,   CASE_NO_BRACES_MULTI_STATEMENT},
            {"Case Braces",                    CASE_WITH_BRACES,                 CASE_NO_BRACES},
            {"Case Braces Multi Statement",    CASE_WITH_BRACES_MULTI_STATEMENT, CASE_NO_BRACES_MULTI_STATEMENT},
            {"Multi Case No Braces",           MULTI_CASE_NO_BRACES,             MULTI_CASE_NO_BRACES},
            {"Multi Case With Braces",         MULTI_CASE_WITH_BRACES,           MULTI_CASE_NO_BRACES},
        }
    },
    {Style::BraceExistance::Require, {
            {"Case No Braces",                 CASE_NO_BRACES,                   CASE_WITH_BRACES},
            {"Case No Braces Multi Statement", CASE_NO_BRACES_MULTI_STATEMENT,   CASE_WITH_BRACES_MULTI_STATEMENT},
            {"Case Braces",                    CASE_WITH_BRACES,                 CASE_WITH_BRACES},
            {"Case Braces Multi Statement",    CASE_WITH_BRACES_MULTI_STATEMENT, CASE_WITH_BRACES_MULTI_STATEMENT},
            {"Multi Case No Braces",           MULTI_CASE_NO_BRACES,             MULTI_CASE_WITH_BRACES},
            {"Multi Case With Braces",         MULTI_CASE_WITH_BRACES,           MULTI_CASE_WITH_BRACES},
        }
    },
};

TEST_CASE("Case Statements") {
    TestContext<Style::BraceExistance> context {
        .styleNames = BRACE_EXISTANCE_NAMES,
        .styleUpdater = [](Style& style, Style::BraceExistance existance) {
            style.braces.caseStatements = existance;
        }
    };

    context.formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    RunTest(context, TEST_CASES);
}
