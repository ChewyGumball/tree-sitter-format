#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/traversers/IndentationTraverser.h>

using namespace tree_sitter_format;

const std::string NONE = R"(
namespace test 
{
std::string s;
}
)";

const std::string BRACES_INDENTED = R"(
namespace test 
    {
    std::string s;
    }
)";

const std::string BODY_INDENTED = R"(
namespace test 
{
    std::string s;
}
)";

const std::string BOTH_INDENTED = R"(
namespace test 
    {
        std::string s;
    }
)";

const std::string NONE_START_SAME_LINE = R"(
namespace test {
std::string s;
}
)";

const std::string BRACES_INDENTED_START_SAME_LINE = R"(
namespace test {
    std::string s;
    }
)";

const std::string BODY_INDENTED_START_SAME_LINE = R"(
namespace test {
    std::string s;
}
)";

const std::string BOTH_INDENTED_START_SAME_LINE = R"(
namespace test {
        std::string s;
    }
)";

const std::string NONE_END_SAME_LINE = R"(
namespace test 
{
std::string s;}
)";

const std::string BRACES_INDENTED_END_SAME_LINE = R"(
namespace test 
    {
    std::string s;}
)";

const std::string BODY_INDENTED_END_SAME_LINE = R"(
namespace test 
{
    std::string s;}
)";

const std::string BOTH_INDENTED_END_SAME_LINE = R"(
namespace test 
    {
        std::string s;}
)";

const std::string BOTH_SAME_LINE = R"(
namespace test {std::string s;}
)";

struct TestCase {
    std::string inputLabel;
    std::string inputValue;

    std::string outputValue;
};

struct TestSuite {
    Style::Indentation style;
    std::string        styleLabel;

    std::vector<TestCase> cases;
};

std::vector<TestSuite> TEST_CASES = {
    {Style::Indentation::None, "None", {
            {"None",                            NONE,                            NONE},
            {"Braces Indented",                 BRACES_INDENTED,                 NONE},
            {"Body Indented",                   BODY_INDENTED,                   NONE},
            {"Both Indented",                   BOTH_INDENTED,                   NONE},
            {"None Start Same Line",            NONE_START_SAME_LINE,            NONE_START_SAME_LINE},
            {"Braces Indented Start Same Line", BRACES_INDENTED_START_SAME_LINE, NONE_START_SAME_LINE},
            {"Body Indented Start Same Line",   BODY_INDENTED_START_SAME_LINE,   NONE_START_SAME_LINE},
            {"Both Indented Start Same Line",   BOTH_INDENTED_START_SAME_LINE,   NONE_START_SAME_LINE},
            {"None End Same Line",              NONE_END_SAME_LINE,              NONE_END_SAME_LINE},
            {"Braces Indented End Same Line",   BRACES_INDENTED_END_SAME_LINE,   NONE_END_SAME_LINE},
            {"Body Indented End Same Line",     BODY_INDENTED_END_SAME_LINE,     NONE_END_SAME_LINE},
            {"Both Indented End Same Line",     BOTH_INDENTED_END_SAME_LINE,     NONE_END_SAME_LINE},
            {"Both Same Line",                  BOTH_SAME_LINE,                  BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BracesIndented, "Braces Indented", {
            {"None",                            NONE,                            BRACES_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BRACES_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BRACES_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BRACES_INDENTED},
            {"None Start Same Line",            NONE_START_SAME_LINE,            BRACES_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", BRACES_INDENTED_START_SAME_LINE, BRACES_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   BODY_INDENTED_START_SAME_LINE,   BRACES_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   BOTH_INDENTED_START_SAME_LINE,   BRACES_INDENTED_START_SAME_LINE},
            {"None End Same Line",              NONE_END_SAME_LINE,              BRACES_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   BRACES_INDENTED_END_SAME_LINE,   BRACES_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     BODY_INDENTED_END_SAME_LINE,     BRACES_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     BOTH_INDENTED_END_SAME_LINE,     BRACES_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  BOTH_SAME_LINE,                  BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BodyIndented, "Body Indented", {
            {"None",                            NONE,                            BODY_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BODY_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BODY_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BODY_INDENTED},
            {"None Start Same Line",            NONE_START_SAME_LINE,            BODY_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", BRACES_INDENTED_START_SAME_LINE, BODY_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   BODY_INDENTED_START_SAME_LINE,   BODY_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   BOTH_INDENTED_START_SAME_LINE,   BODY_INDENTED_START_SAME_LINE},
            {"None End Same Line",              NONE_END_SAME_LINE,              BODY_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   BRACES_INDENTED_END_SAME_LINE,   BODY_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     BODY_INDENTED_END_SAME_LINE,     BODY_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     BOTH_INDENTED_END_SAME_LINE,     BODY_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  BOTH_SAME_LINE,                  BOTH_SAME_LINE},
        }
    },
    {Style::Indentation::BothIndented, "Both Indented", {
            {"None",                            NONE,                            BOTH_INDENTED},
            {"Braces Indented",                 BRACES_INDENTED,                 BOTH_INDENTED},
            {"Body Indented",                   BODY_INDENTED,                   BOTH_INDENTED},
            {"Both Indented",                   BOTH_INDENTED,                   BOTH_INDENTED},
            {"None Start Same Line",            NONE_START_SAME_LINE,            BOTH_INDENTED_START_SAME_LINE},
            {"Braces Indented Start Same Line", BRACES_INDENTED_START_SAME_LINE, BOTH_INDENTED_START_SAME_LINE},
            {"Body Indented Start Same Line",   BODY_INDENTED_START_SAME_LINE,   BOTH_INDENTED_START_SAME_LINE},
            {"Both Indented Start Same Line",   BOTH_INDENTED_START_SAME_LINE,   BOTH_INDENTED_START_SAME_LINE},
            {"None End Same Line",              NONE_END_SAME_LINE,              BOTH_INDENTED_END_SAME_LINE},
            {"Braces Indented End Same Line",   BRACES_INDENTED_END_SAME_LINE,   BOTH_INDENTED_END_SAME_LINE},
            {"Body Indented End Same Line",     BODY_INDENTED_END_SAME_LINE,     BOTH_INDENTED_END_SAME_LINE},
            {"Both Indented End Same Line",     BOTH_INDENTED_END_SAME_LINE,     BOTH_INDENTED_END_SAME_LINE},
            {"Both Same Line",                  BOTH_SAME_LINE,                  BOTH_SAME_LINE},
        }
    },
};


TEST_CASE("Namespaces") {
    Formatter formatter;
    formatter.addTraverser(std::make_unique<IndentationTraverser>());

    Style style;
    style.indentation.whitespace = Style::IndentationWhitespace::Spaces;

    for(TestSuite& suite : TEST_CASES) {
        SECTION(suite.styleLabel) {
            style.indentation.namespaces = suite.style;

            for(TestCase& testCase : suite.cases) {
                SECTION(testCase.inputLabel) {
                    Document document(testCase.inputValue);
                    formatter.format(style, document);

                    REQUIRE(document.toString() == testCase.outputValue);
                }
            }
        }
    }
}
