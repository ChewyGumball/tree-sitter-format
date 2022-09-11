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
b = !a;
return b;
)";

const std::string REAL_MULTI_IF = R"(
if(true)
{b = !a;}
return b;
)";

const std::string IF_WITH_BRACES_ELSEIF_WITH_BRACES = R"(
if(true) 
    {b = !a;}
else if (false)
    {b = a;}
)";

const std::string IF_NO_BRACES_ELSEIF_NO_BRACES = R"(
if(true) 
    b = !a;
else if (false)
    b = a;
)";



std::vector<TestSuite<Style::BraceExistance>> TEST_CASES = {
    {Style::BraceExistance::Ignore, {
            {"If No Braces",                            IF_NO_BRACES,                      IF_NO_BRACES},
            {"If Braces",                               IF_WITH_BRACES,                    IF_WITH_BRACES},
            {"If Multi Statement",                      MULTI_IF,                          MULTI_IF},
            {"If No Braces Else No Braces",             IF_NO_BRACES_ELSE_NO_BRACES,       IF_NO_BRACES_ELSE_NO_BRACES},
            {"If No Braces Else Braces",                IF_NO_BRACES_ELSE_WITH_BRACES,     IF_NO_BRACES_ELSE_WITH_BRACES},
            {"If Braces Else No Braces",                IF_WITH_BRACES_ELSE_NO_BRACES,     IF_WITH_BRACES_ELSE_NO_BRACES},
            {"If Braces Else Braces",                   IF_WITH_BRACES_ELSE_WITH_BRACES,   IF_WITH_BRACES_ELSE_WITH_BRACES},
            {"If Multi Statement Else Multi Statement", MULTI_IF_ELSE,                     MULTI_IF_ELSE},
            {"If Fake Multi",                           FAKE_MULTI_IF,                     FAKE_MULTI_IF},
            {"If Real Multi",                           REAL_MULTI_IF,                     REAL_MULTI_IF},
            {"If Braces ElseIf Braces",                 IF_WITH_BRACES_ELSEIF_WITH_BRACES, IF_WITH_BRACES_ELSEIF_WITH_BRACES},
            {"If No Braces ElseIf No Braces",           IF_NO_BRACES_ELSEIF_NO_BRACES,     IF_NO_BRACES_ELSEIF_NO_BRACES},
        }
    },
    {Style::BraceExistance::Remove, {
            {"If No Braces",                            IF_NO_BRACES,                      IF_NO_BRACES},
            {"If Braces",                               IF_WITH_BRACES,                    IF_NO_BRACES},
            {"If Multi Statement",                      MULTI_IF,                          MULTI_IF},
            {"If No Braces Else No Braces",             IF_NO_BRACES_ELSE_NO_BRACES,       IF_NO_BRACES_ELSE_NO_BRACES},
            {"If No Braces Else Braces",                IF_NO_BRACES_ELSE_WITH_BRACES,     IF_NO_BRACES_ELSE_NO_BRACES},
            {"If Braces Else No Braces",                IF_WITH_BRACES_ELSE_NO_BRACES,     IF_NO_BRACES_ELSE_NO_BRACES},
            {"If Braces Else Braces",                   IF_WITH_BRACES_ELSE_WITH_BRACES,   IF_NO_BRACES_ELSE_NO_BRACES},
            {"If Multi Statement Else Multi Statement", MULTI_IF_ELSE,                     MULTI_IF_ELSE},
            {"If Fake Multi",                           FAKE_MULTI_IF,                     FAKE_MULTI_IF},
            {"If Real Multi",                           REAL_MULTI_IF,                     FAKE_MULTI_IF},
            {"If Braces ElseIf Braces",                 IF_WITH_BRACES_ELSEIF_WITH_BRACES, IF_NO_BRACES_ELSEIF_NO_BRACES},
            {"If No Braces ElseIf No Braces",           IF_NO_BRACES_ELSEIF_NO_BRACES,     IF_NO_BRACES_ELSEIF_NO_BRACES},
        }
    },
    {Style::BraceExistance::Require, {
            {"If No Braces",                            IF_NO_BRACES,                      IF_WITH_BRACES},
            {"If Braces",                               IF_WITH_BRACES,                    IF_WITH_BRACES},
            {"If Multi Statement",                      MULTI_IF,                          MULTI_IF},
            {"If No Braces Else No Braces",             IF_NO_BRACES_ELSE_NO_BRACES,       IF_WITH_BRACES_ELSE_WITH_BRACES},
            {"If No Braces Else Braces",                IF_NO_BRACES_ELSE_WITH_BRACES,     IF_WITH_BRACES_ELSE_WITH_BRACES},
            {"If Braces Else No Braces",                IF_WITH_BRACES_ELSE_NO_BRACES,     IF_WITH_BRACES_ELSE_WITH_BRACES},
            {"If Braces Else Braces",                   IF_WITH_BRACES_ELSE_WITH_BRACES,   IF_WITH_BRACES_ELSE_WITH_BRACES},
            {"If Multi Statement Else Multi Statement", MULTI_IF_ELSE,                     MULTI_IF_ELSE},
            {"If Fake Multi",                           FAKE_MULTI_IF,                     REAL_MULTI_IF},
            {"If Real Multi",                           REAL_MULTI_IF,                     REAL_MULTI_IF},
            {"If Braces ElseIf Braces",                 IF_WITH_BRACES_ELSEIF_WITH_BRACES, IF_WITH_BRACES_ELSEIF_WITH_BRACES},
            {"If No Braces ElseIf No Braces",           IF_NO_BRACES_ELSEIF_NO_BRACES,     IF_WITH_BRACES_ELSEIF_WITH_BRACES},
        }
    },
};

TEST_CASE("If Statements") {
    TestContext<Style::BraceExistance> context {
        .styleNames = BRACE_EXISTANCE_NAMES,
        .styleUpdater = [](Style& style, Style::BraceExistance existance) {
            style.braces.ifStatements = existance;
        }
    };

    context.formatter.addTraverser(std::make_unique<BracketExistanceTraverser>());

    RunTest(context, TEST_CASES);
}
