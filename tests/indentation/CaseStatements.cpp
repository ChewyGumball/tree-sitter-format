#include <catch2/catch_test_macros.hpp>

#include <tree-sitter-format/style/Style.h>
#include <tree-sitter-format/traversers/IndentationTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;

// ----------------------------------------------- //
//                No Compound Body                 //
// ----------------------------------------------- //

const std::string NONE = R"(
case 5:
f();
f();
)";

const std::string BRACES_INDENTED = R"(
case 5:
    f();
    f();
)";

const std::string BODY_INDENTED = R"(
case 5:
    f();
    f();
)";

const std::string BOTH_INDENTED = R"(
case 5:
    f();
    f();
)";

std::vector<TestSuite<Style::Indentation>> TEST_CASES_NO_COMPOUND = {
    {Style::Indentation::None, {
            {"None",                            NONE,                            NONE},
            {"Braces Indented",                 BRACES_INDENTED,                 NONE},
            {"Body Indented",                   BODY_INDENTED,                   NONE},
            {"Both Indented",                   BOTH_INDENTED,                   NONE},
        }
    },
    {Style::Indentation::BracesIndented, {
            {"None",                            NONE,                            BRACES_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BRACES_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BRACES_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BRACES_INDENTED},
        }
    },
    {Style::Indentation::BodyIndented, {
            {"None",                            NONE,                            BODY_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BODY_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BODY_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BODY_INDENTED},
        }
    },
    {Style::Indentation::BothIndented, {
            {"None",                            NONE,                            BOTH_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BOTH_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BOTH_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BOTH_INDENTED},
        }
    },
};


TEST_CASE("No Compound Body Case Statement") {
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.caseBlocks = indentation;
        }
    };

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    RunTest(context, TEST_CASES_NO_COMPOUND);
}

// ----------------------------------------------- //
//              Single Compound Body               //
// ----------------------------------------------- //

const std::string NONE_SINGLE_COMPOUND = R"(
case 5:
{
f();
f();
}
)";

const std::string BRACES_INDENTED_SINGLE_COMPOUND = R"(
case 5:
    {
    f();
    f();
    }
)";

const std::string BODY_INDENTED_SINGLE_COMPOUND = R"(
case 5:
{
    f();
    f();
}
)";

const std::string BOTH_INDENTED_SINGLE_COMPOUND = R"(
case 5:
    {
        f();
        f();
    }
)";

std::vector<TestSuite<Style::Indentation>> TEST_CASES_SINGLE_COMPOUND = {
    {Style::Indentation::None, {
            {"None",            NONE_SINGLE_COMPOUND,            NONE_SINGLE_COMPOUND},
            {"Braces Indented", BRACES_INDENTED_SINGLE_COMPOUND, NONE_SINGLE_COMPOUND},
            {"Body Indented",   BODY_INDENTED_SINGLE_COMPOUND,   NONE_SINGLE_COMPOUND},
            {"Both Indented",   BOTH_INDENTED_SINGLE_COMPOUND,   NONE_SINGLE_COMPOUND},
        }
    },
    {Style::Indentation::BracesIndented, {
            {"None",            NONE_SINGLE_COMPOUND,            BRACES_INDENTED_SINGLE_COMPOUND},
            {"Braces Indented", BRACES_INDENTED_SINGLE_COMPOUND, BRACES_INDENTED_SINGLE_COMPOUND},
            {"Body Indented",   BODY_INDENTED_SINGLE_COMPOUND,   BRACES_INDENTED_SINGLE_COMPOUND},
            {"Both Indented",   BOTH_INDENTED_SINGLE_COMPOUND,   BRACES_INDENTED_SINGLE_COMPOUND},
        }
    },
    {Style::Indentation::BodyIndented, {
            {"None",            NONE_SINGLE_COMPOUND,            BODY_INDENTED_SINGLE_COMPOUND},
            {"Braces Indented", BRACES_INDENTED_SINGLE_COMPOUND, BODY_INDENTED_SINGLE_COMPOUND},
            {"Body Indented",   BODY_INDENTED_SINGLE_COMPOUND,   BODY_INDENTED_SINGLE_COMPOUND},
            {"Both Indented",   BOTH_INDENTED_SINGLE_COMPOUND,   BODY_INDENTED_SINGLE_COMPOUND},
        }
    },
    {Style::Indentation::BothIndented, {
            {"None",            NONE_SINGLE_COMPOUND,            BOTH_INDENTED_SINGLE_COMPOUND},
            {"Braces Indented", BRACES_INDENTED_SINGLE_COMPOUND, BOTH_INDENTED_SINGLE_COMPOUND},
            {"Body Indented",   BODY_INDENTED_SINGLE_COMPOUND,   BOTH_INDENTED_SINGLE_COMPOUND},
            {"Both Indented",   BOTH_INDENTED_SINGLE_COMPOUND,   BOTH_INDENTED_SINGLE_COMPOUND},
        }
    },
};


TEST_CASE("Single Compound Body Case Statement") {
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.caseBlocks = indentation;
        }
    };

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    RunTest(context, TEST_CASES_SINGLE_COMPOUND);
}

// ----------------------------------------------- //
//               Multi Compound Body               //
// ----------------------------------------------- //

const std::string NONE_MULTI_COMPOUND = R"(
case 5:
{
f();
f();
}
break;
)";

const std::string BRACES_INDENTED_MULTI_COMPOUND = R"(
case 5:
    {
    f();
    f();
    }
    break;
)";

const std::string BODY_INDENTED_MULTI_COMPOUND = R"(
case 5:
    {
    f();
    f();
    }
    break;
)";

const std::string BOTH_INDENTED_MULTI_COMPOUND = R"(
case 5:
    {
    f();
    f();
    }
    break;
)";

std::vector<TestSuite<Style::Indentation>> TEST_CASES_MULTI_COMPOUND = {
    {Style::Indentation::None, {
            {"None",            NONE_MULTI_COMPOUND,            NONE_MULTI_COMPOUND},
            {"Braces Indented", BRACES_INDENTED_MULTI_COMPOUND, NONE_MULTI_COMPOUND},
            {"Body Indented",   BODY_INDENTED_MULTI_COMPOUND,   NONE_MULTI_COMPOUND},
            {"Both Indented",   BOTH_INDENTED_MULTI_COMPOUND,   NONE_MULTI_COMPOUND},
        }
    },
    {Style::Indentation::BracesIndented, {
            {"None",            NONE_MULTI_COMPOUND,            BRACES_INDENTED_MULTI_COMPOUND},
            {"Braces Indented", BRACES_INDENTED_MULTI_COMPOUND, BRACES_INDENTED_MULTI_COMPOUND},
            {"Body Indented",   BODY_INDENTED_MULTI_COMPOUND,   BRACES_INDENTED_MULTI_COMPOUND},
            {"Both Indented",   BOTH_INDENTED_MULTI_COMPOUND,   BRACES_INDENTED_MULTI_COMPOUND},
        }
    },
    {Style::Indentation::BodyIndented, {
            {"None",            NONE_MULTI_COMPOUND,            BODY_INDENTED_MULTI_COMPOUND},
            {"Braces Indented", BRACES_INDENTED_MULTI_COMPOUND, BODY_INDENTED_MULTI_COMPOUND},
            {"Body Indented",   BODY_INDENTED_MULTI_COMPOUND,   BODY_INDENTED_MULTI_COMPOUND},
            {"Both Indented",   BOTH_INDENTED_MULTI_COMPOUND,   BODY_INDENTED_MULTI_COMPOUND},
        }
    },
    {Style::Indentation::BothIndented, {
            {"None",            NONE_MULTI_COMPOUND,            BOTH_INDENTED_MULTI_COMPOUND},
            {"Braces Indented", BRACES_INDENTED_MULTI_COMPOUND, BOTH_INDENTED_MULTI_COMPOUND},
            {"Body Indented",   BODY_INDENTED_MULTI_COMPOUND,   BOTH_INDENTED_MULTI_COMPOUND},
            {"Both Indented",   BOTH_INDENTED_MULTI_COMPOUND,   BOTH_INDENTED_MULTI_COMPOUND},
        }
    },
};


TEST_CASE("Multi Compound Body Case Statement") {
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.caseBlocks = indentation;
        }
    };

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;
    context.style.indentation.genericScope = Style::Indentation::None;

    RunTest(context, TEST_CASES_MULTI_COMPOUND);
}
