#include <catch2/catch_test_macros.hpp>

#include <tree-sitter-format/style/Style.h>
#include <tree-sitter-format/traversers/IndentationTraverser.h>
#include <tests/TestUtils.h>

using namespace tree_sitter_format;
using namespace testing;


// ----------------------------------------------- //
//                   Just if's                     //
// ----------------------------------------------- //

const std::string IF_NONE = R"(
if(false)
{
int a;
}
)";

const std::string IF_BRACES_INDENTED = R"(
if(false)
    {
    int a;
    }
)";

const std::string IF_BODY_INDENTED = R"(
if(false)
{
    int a;
}
)";

const std::string IF_BOTH_INDENTED = R"(
if(false)
    {
        int a;
    }
)";

const std::string IF_NONE_START_SAME_LINE = R"(
if(false) {
int a;
}
)";

const std::string IF_BRACES_INDENTED_START_SAME_LINE = R"(
if(false) {
    int a;
    }
)";

const std::string IF_BODY_INDENTED_START_SAME_LINE = R"(
if(false) {
    int a;
}
)";

const std::string IF_BOTH_INDENTED_START_SAME_LINE = R"(
if(false) {
        int a;
    }
)";

const std::string IF_NONE_END_SAME_LINE = R"(
if(false)
{
int a;}
)";

const std::string IF_BRACES_INDENTED_END_SAME_LINE = R"(
if(false)
    {
    int a;}
)";

const std::string IF_BODY_INDENTED_END_SAME_LINE = R"(
if(false)
{
    int a;}
)";

const std::string IF_BOTH_INDENTED_END_SAME_LINE = R"(
if(false)
    {
        int a;}
)";

const std::string IF_BOTH_SAME_LINE = R"(
if(false) {int a;}
)";

std::vector<TestSuite<Style::Indentation>> IF_TEST_CASES {
    {Style::Indentation::None, {
            {"None",                            IF_NONE,                            IF_NONE},
            {"Braces Indented",                 IF_BRACES_INDENTED,                 IF_NONE},
            {"Body Indented",                   IF_BODY_INDENTED,                   IF_NONE},
            {"Both Indented",                   IF_BOTH_INDENTED,                   IF_NONE},
            {"None Start Same Line",            IF_NONE_START_SAME_LINE,            IF_NONE_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_BRACES_INDENTED_START_SAME_LINE, IF_NONE_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_BODY_INDENTED_START_SAME_LINE,   IF_NONE_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_BOTH_INDENTED_START_SAME_LINE,   IF_NONE_START_SAME_LINE},
            {"None End Same Line",              IF_NONE_END_SAME_LINE,              IF_NONE_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_BRACES_INDENTED_END_SAME_LINE,   IF_NONE_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_BODY_INDENTED_END_SAME_LINE,     IF_NONE_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_BOTH_INDENTED_END_SAME_LINE,     IF_NONE_END_SAME_LINE},
            {"Both Same Line",                  IF_BOTH_SAME_LINE,                  IF_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BracesIndented, {
            {"None",                            IF_NONE,                            IF_BRACES_INDENTED},
            {"Braces Indented",                 IF_BRACES_INDENTED,                 IF_BRACES_INDENTED},
            {"Body Indented",                   IF_BODY_INDENTED,                   IF_BRACES_INDENTED},
            {"Both Indented",                   IF_BOTH_INDENTED,                   IF_BRACES_INDENTED},
            {"None Start Same Line",            IF_NONE_START_SAME_LINE,            IF_BRACES_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_BRACES_INDENTED_START_SAME_LINE, IF_BRACES_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_BODY_INDENTED_START_SAME_LINE,   IF_BRACES_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_BOTH_INDENTED_START_SAME_LINE,   IF_BRACES_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_NONE_END_SAME_LINE,              IF_BRACES_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_BRACES_INDENTED_END_SAME_LINE,   IF_BRACES_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_BODY_INDENTED_END_SAME_LINE,     IF_BRACES_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_BOTH_INDENTED_END_SAME_LINE,     IF_BRACES_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_BOTH_SAME_LINE,                  IF_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BodyIndented, {
            {"None",                            IF_NONE,                            IF_BODY_INDENTED},
            {"Braces Indented",                 IF_BRACES_INDENTED,                 IF_BODY_INDENTED},
            {"Body Indented",                   IF_BODY_INDENTED,                   IF_BODY_INDENTED},
            {"Both Indented",                   IF_BOTH_INDENTED,                   IF_BODY_INDENTED},
            {"None Start Same Line",            IF_NONE_START_SAME_LINE,            IF_BODY_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_BRACES_INDENTED_START_SAME_LINE, IF_BODY_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_BODY_INDENTED_START_SAME_LINE,   IF_BODY_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_BOTH_INDENTED_START_SAME_LINE,   IF_BODY_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_NONE_END_SAME_LINE,              IF_BODY_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_BRACES_INDENTED_END_SAME_LINE,   IF_BODY_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_BODY_INDENTED_END_SAME_LINE,     IF_BODY_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_BOTH_INDENTED_END_SAME_LINE,     IF_BODY_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_BOTH_SAME_LINE,                  IF_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BothIndented, {
            {"None",                            IF_NONE,                            IF_BOTH_INDENTED},
            {"Braces Indented",                 IF_BRACES_INDENTED,                 IF_BOTH_INDENTED},
            {"Body Indented",                   IF_BODY_INDENTED,                   IF_BOTH_INDENTED},
            {"Both Indented",                   IF_BOTH_INDENTED,                   IF_BOTH_INDENTED},
            {"None Start Same Line",            IF_NONE_START_SAME_LINE,            IF_BOTH_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_BRACES_INDENTED_START_SAME_LINE, IF_BOTH_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_BODY_INDENTED_START_SAME_LINE,   IF_BOTH_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_BOTH_INDENTED_START_SAME_LINE,   IF_BOTH_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_NONE_END_SAME_LINE,              IF_BOTH_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_BRACES_INDENTED_END_SAME_LINE,   IF_BOTH_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_BODY_INDENTED_END_SAME_LINE,     IF_BOTH_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_BOTH_INDENTED_END_SAME_LINE,     IF_BOTH_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_BOTH_SAME_LINE,                  IF_BOTH_SAME_LINE},
        }
    },
};


TEST_CASE("If Statements") {    
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.ifStatements = indentation;
        }
    };

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    RunTest(context, IF_TEST_CASES);
}

// ----------------------------------------------- //
//                   if ... else                   //
// ----------------------------------------------- //

const std::string IF_ELSE_NONE = R"(
if(false)
{
int a;
}
else
{
int b;
}
)";

const std::string IF_ELSE_BRACES_INDENTED = R"(
if(false)
    {
    int a;
    }
else
    {
    int b;
    }
)";

const std::string IF_ELSE_BODY_INDENTED = R"(
if(false)
{
    int a;
}
else
{
    int b;
}
)";

const std::string IF_ELSE_BOTH_INDENTED = R"(
if(false)
    {
        int a;
    }
else
    {
        int b;
    }
)";

const std::string IF_ELSE_NONE_START_SAME_LINE = R"(
if(false) {
int a;
} else {
int b;
}
)";

const std::string IF_ELSE_BRACES_INDENTED_START_SAME_LINE = R"(
if(false) {
    int a;
    } else {
    int b;
    }
)";

const std::string IF_ELSE_BODY_INDENTED_START_SAME_LINE = R"(
if(false) {
    int a;
} else {
    int b;
}
)";

const std::string IF_ELSE_BOTH_INDENTED_START_SAME_LINE = R"(
if(false) {
        int a;
    } else {
        int b;
    }
)";

const std::string IF_ELSE_NONE_END_SAME_LINE = R"(
if(false)
{
int a;} else
{
int b;}
)";

const std::string IF_ELSE_BRACES_INDENTED_END_SAME_LINE = R"(
if(false)
    {
    int a;} else
    {
    int b;}
)";

const std::string IF_ELSE_BODY_INDENTED_END_SAME_LINE = R"(
if(false)
{
    int a;} else
{
    int b;}
)";

const std::string IF_ELSE_BOTH_INDENTED_END_SAME_LINE = R"(
if(false)
    {
        int a;} else
    {
        int b;}
)";

const std::string IF_ELSE_BOTH_SAME_LINE = R"(
if(false) {int a;}
else {int b;}
)";

std::vector<TestSuite<Style::Indentation>> IF_ELSE_TEST_CASES {
    {Style::Indentation::None, {
            {"None",                            IF_ELSE_NONE,                            IF_ELSE_NONE},
            {"Braces Indented",                 IF_ELSE_BRACES_INDENTED,                 IF_ELSE_NONE},
            {"Body Indented",                   IF_ELSE_BODY_INDENTED,                   IF_ELSE_NONE},
            {"Both Indented",                   IF_ELSE_BOTH_INDENTED,                   IF_ELSE_NONE},
            {"None Start Same Line",            IF_ELSE_NONE_START_SAME_LINE,            IF_ELSE_NONE_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_ELSE_BRACES_INDENTED_START_SAME_LINE, IF_ELSE_NONE_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_ELSE_BODY_INDENTED_START_SAME_LINE,   IF_ELSE_NONE_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_ELSE_BOTH_INDENTED_START_SAME_LINE,   IF_ELSE_NONE_START_SAME_LINE},
            {"None End Same Line",              IF_ELSE_NONE_END_SAME_LINE,              IF_ELSE_NONE_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_ELSE_BRACES_INDENTED_END_SAME_LINE,   IF_ELSE_NONE_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_ELSE_BODY_INDENTED_END_SAME_LINE,     IF_ELSE_NONE_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_ELSE_BOTH_INDENTED_END_SAME_LINE,     IF_ELSE_NONE_END_SAME_LINE},
            {"Both Same Line",                  IF_ELSE_BOTH_SAME_LINE,                  IF_ELSE_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BracesIndented, {
            {"None",                            IF_ELSE_NONE,                            IF_ELSE_BRACES_INDENTED},
            {"Braces Indented",                 IF_ELSE_BRACES_INDENTED,                 IF_ELSE_BRACES_INDENTED},
            {"Body Indented",                   IF_ELSE_BODY_INDENTED,                   IF_ELSE_BRACES_INDENTED},
            {"Both Indented",                   IF_ELSE_BOTH_INDENTED,                   IF_ELSE_BRACES_INDENTED},
            {"None Start Same Line",            IF_ELSE_NONE_START_SAME_LINE,            IF_ELSE_BRACES_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_ELSE_BRACES_INDENTED_START_SAME_LINE, IF_ELSE_BRACES_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_ELSE_BODY_INDENTED_START_SAME_LINE,   IF_ELSE_BRACES_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_ELSE_BOTH_INDENTED_START_SAME_LINE,   IF_ELSE_BRACES_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_ELSE_NONE_END_SAME_LINE,              IF_ELSE_BRACES_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_ELSE_BRACES_INDENTED_END_SAME_LINE,   IF_ELSE_BRACES_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_ELSE_BODY_INDENTED_END_SAME_LINE,     IF_ELSE_BRACES_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_ELSE_BOTH_INDENTED_END_SAME_LINE,     IF_ELSE_BRACES_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_ELSE_BOTH_SAME_LINE,                  IF_ELSE_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BodyIndented, {
            {"None",                            IF_ELSE_NONE,                            IF_ELSE_BODY_INDENTED},
            {"Braces Indented",                 IF_ELSE_BRACES_INDENTED,                 IF_ELSE_BODY_INDENTED},
            {"Body Indented",                   IF_ELSE_BODY_INDENTED,                   IF_ELSE_BODY_INDENTED},
            {"Both Indented",                   IF_ELSE_BOTH_INDENTED,                   IF_ELSE_BODY_INDENTED},
            {"None Start Same Line",            IF_ELSE_NONE_START_SAME_LINE,            IF_ELSE_BODY_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_ELSE_BRACES_INDENTED_START_SAME_LINE, IF_ELSE_BODY_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_ELSE_BODY_INDENTED_START_SAME_LINE,   IF_ELSE_BODY_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_ELSE_BOTH_INDENTED_START_SAME_LINE,   IF_ELSE_BODY_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_ELSE_NONE_END_SAME_LINE,              IF_ELSE_BODY_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_ELSE_BRACES_INDENTED_END_SAME_LINE,   IF_ELSE_BODY_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_ELSE_BODY_INDENTED_END_SAME_LINE,     IF_ELSE_BODY_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_ELSE_BOTH_INDENTED_END_SAME_LINE,     IF_ELSE_BODY_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_ELSE_BOTH_SAME_LINE,                  IF_ELSE_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BothIndented, {
            {"None",                            IF_ELSE_NONE,                            IF_ELSE_BOTH_INDENTED},
            {"Braces Indented",                 IF_ELSE_BRACES_INDENTED,                 IF_ELSE_BOTH_INDENTED},
            {"Body Indented",                   IF_ELSE_BODY_INDENTED,                   IF_ELSE_BOTH_INDENTED},
            {"Both Indented",                   IF_ELSE_BOTH_INDENTED,                   IF_ELSE_BOTH_INDENTED},
            {"None Start Same Line",            IF_ELSE_NONE_START_SAME_LINE,            IF_ELSE_BOTH_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_ELSE_BRACES_INDENTED_START_SAME_LINE, IF_ELSE_BOTH_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_ELSE_BODY_INDENTED_START_SAME_LINE,   IF_ELSE_BOTH_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_ELSE_BOTH_INDENTED_START_SAME_LINE,   IF_ELSE_BOTH_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_ELSE_NONE_END_SAME_LINE,              IF_ELSE_BOTH_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_ELSE_BRACES_INDENTED_END_SAME_LINE,   IF_ELSE_BOTH_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_ELSE_BODY_INDENTED_END_SAME_LINE,     IF_ELSE_BOTH_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_ELSE_BOTH_INDENTED_END_SAME_LINE,     IF_ELSE_BOTH_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_ELSE_BOTH_SAME_LINE,                  IF_ELSE_BOTH_SAME_LINE},
        }
    },
};


TEST_CASE("If Else Statements") {    
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.ifStatements = indentation;
        }
    };

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    RunTest(context, IF_ELSE_TEST_CASES);
}

// ----------------------------------------------- //
//              if ... else if .. else             //
// ----------------------------------------------- //

const std::string IF_ELSEIF_ELSE_NONE = R"(
if(false)
{
int a;
}
else if(true)
{
int c;
}
else
{
int b;
}
)";

const std::string IF_ELSEIF_ELSE_BRACES_INDENTED = R"(
if(false)
    {
    int a;
    }
else if(true)
    {
    int c;
    }
else
    {
    int b;
    }
)";

const std::string IF_ELSEIF_ELSE_BODY_INDENTED = R"(
if(false)
{
    int a;
}
else if(true)
{
    int c;
}
else
{
    int b;
}
)";

const std::string IF_ELSEIF_ELSE_BOTH_INDENTED = R"(
if(false)
    {
        int a;
    }
else if(true)
    {
        int c;
    }
else
    {
        int b;
    }
)";

const std::string IF_ELSEIF_ELSE_NONE_START_SAME_LINE = R"(
if(false) {
int a;
} else if(true) {
int c;
} else {
int b;
}
)";

const std::string IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE = R"(
if(false) {
    int a;
    } else if(true) {
    int c;
    } else {
    int b;
    }
)";

const std::string IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE = R"(
if(false) {
    int a;
} else if(true) {
    int c;
} else {
    int b;
}
)";

const std::string IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE = R"(
if(false) {
        int a;
    } else if(true) {
        int c;
    } else {
        int b;
    }
)";

const std::string IF_ELSEIF_ELSE_NONE_END_SAME_LINE = R"(
if(false)
{
int a;} else if(true)
{
int c;} else
{
int b;}
)";

const std::string IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE = R"(
if(false)
    {
    int a;} else if(true)
    {
    int c;} else
    {
    int b;}
)";

const std::string IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE = R"(
if(false)
{
    int a;} else if(true)
{
    int c;} else
{
    int b;}
)";

const std::string IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE = R"(
if(false)
    {
        int a;} else if(true)
    {
        int c;} else
    {
        int b;}
)";

const std::string IF_ELSEIF_ELSE_BOTH_SAME_LINE = R"(
if(false) {int a;}
else if(true) {int c;}
else {int b;}
)";

std::vector<TestSuite<Style::Indentation>> IF_ELSEIF_ELSE_TEST_CASES {
    {Style::Indentation::None, {
            {"None",                            IF_ELSEIF_ELSE_NONE,                            IF_ELSEIF_ELSE_NONE},
            {"Braces Indented",                 IF_ELSEIF_ELSE_BRACES_INDENTED,                 IF_ELSEIF_ELSE_NONE},
            {"Body Indented",                   IF_ELSEIF_ELSE_BODY_INDENTED,                   IF_ELSEIF_ELSE_NONE},
            {"Both Indented",                   IF_ELSEIF_ELSE_BOTH_INDENTED,                   IF_ELSEIF_ELSE_NONE},
            {"None Start Same Line",            IF_ELSEIF_ELSE_NONE_START_SAME_LINE,            IF_ELSEIF_ELSE_NONE_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE, IF_ELSEIF_ELSE_NONE_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE,   IF_ELSEIF_ELSE_NONE_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE,   IF_ELSEIF_ELSE_NONE_START_SAME_LINE},
            {"None End Same Line",              IF_ELSEIF_ELSE_NONE_END_SAME_LINE,              IF_ELSEIF_ELSE_NONE_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE,   IF_ELSEIF_ELSE_NONE_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE,     IF_ELSEIF_ELSE_NONE_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE,     IF_ELSEIF_ELSE_NONE_END_SAME_LINE},
            {"Both Same Line",                  IF_ELSEIF_ELSE_BOTH_SAME_LINE,                  IF_ELSEIF_ELSE_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BracesIndented, {
            {"None",                            IF_ELSEIF_ELSE_NONE,                            IF_ELSEIF_ELSE_BRACES_INDENTED},
            {"Braces Indented",                 IF_ELSEIF_ELSE_BRACES_INDENTED,                 IF_ELSEIF_ELSE_BRACES_INDENTED},
            {"Body Indented",                   IF_ELSEIF_ELSE_BODY_INDENTED,                   IF_ELSEIF_ELSE_BRACES_INDENTED},
            {"Both Indented",                   IF_ELSEIF_ELSE_BOTH_INDENTED,                   IF_ELSEIF_ELSE_BRACES_INDENTED},
            {"None Start Same Line",            IF_ELSEIF_ELSE_NONE_START_SAME_LINE,            IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE, IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE,   IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE,   IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_ELSEIF_ELSE_NONE_END_SAME_LINE,              IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE,   IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE,     IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE,     IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_ELSEIF_ELSE_BOTH_SAME_LINE,                  IF_ELSEIF_ELSE_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BodyIndented, {
            {"None",                            IF_ELSEIF_ELSE_NONE,                            IF_ELSEIF_ELSE_BODY_INDENTED},
            {"Braces Indented",                 IF_ELSEIF_ELSE_BRACES_INDENTED,                 IF_ELSEIF_ELSE_BODY_INDENTED},
            {"Body Indented",                   IF_ELSEIF_ELSE_BODY_INDENTED,                   IF_ELSEIF_ELSE_BODY_INDENTED},
            {"Both Indented",                   IF_ELSEIF_ELSE_BOTH_INDENTED,                   IF_ELSEIF_ELSE_BODY_INDENTED},
            {"None Start Same Line",            IF_ELSEIF_ELSE_NONE_START_SAME_LINE,            IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE, IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE,   IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE,   IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_ELSEIF_ELSE_NONE_END_SAME_LINE,              IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE,   IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE,     IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE,     IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_ELSEIF_ELSE_BOTH_SAME_LINE,                  IF_ELSEIF_ELSE_BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BothIndented, {
            {"None",                            IF_ELSEIF_ELSE_NONE,                            IF_ELSEIF_ELSE_BOTH_INDENTED},
            {"Braces Indented",                 IF_ELSEIF_ELSE_BRACES_INDENTED,                 IF_ELSEIF_ELSE_BOTH_INDENTED},
            {"Body Indented",                   IF_ELSEIF_ELSE_BODY_INDENTED,                   IF_ELSEIF_ELSE_BOTH_INDENTED},
            {"Both Indented",                   IF_ELSEIF_ELSE_BOTH_INDENTED,                   IF_ELSEIF_ELSE_BOTH_INDENTED},
            {"None Start Same Line",            IF_ELSEIF_ELSE_NONE_START_SAME_LINE,            IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", IF_ELSEIF_ELSE_BRACES_INDENTED_START_SAME_LINE, IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   IF_ELSEIF_ELSE_BODY_INDENTED_START_SAME_LINE,   IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE,   IF_ELSEIF_ELSE_BOTH_INDENTED_START_SAME_LINE},
            {"None End Same Line",              IF_ELSEIF_ELSE_NONE_END_SAME_LINE,              IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   IF_ELSEIF_ELSE_BRACES_INDENTED_END_SAME_LINE,   IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     IF_ELSEIF_ELSE_BODY_INDENTED_END_SAME_LINE,     IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE,     IF_ELSEIF_ELSE_BOTH_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  IF_ELSEIF_ELSE_BOTH_SAME_LINE,                  IF_ELSEIF_ELSE_BOTH_SAME_LINE},
        }
    },
};


TEST_CASE("If ElseIf Else Statements") {    
    TestContext<Style::Indentation> context {
        .styleNames = INDENTATION_NAMES,
        .styleUpdater = [](Style& style, Style::Indentation indentation) {
            style.indentation.ifStatements = indentation;
        }
    };

    context.formatter.addTraverser(std::make_unique<IndentationTraverser>());
    context.style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    RunTest(context, IF_ELSEIF_ELSE_TEST_CASES);
}