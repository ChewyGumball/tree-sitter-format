#pragma once

#include <tree_sitter_format/Formatter.h>
#include <tree_sitter_format/style/Style.h>

#include <catch2/catch_test_macros.hpp>

#include <unordered_map>
#include <vector>

namespace testing {

inline const std::unordered_map<tree_sitter_format::Style::Indentation, std::string> INDENTATION_NAMES = {
    {tree_sitter_format::Style::Indentation::BracesIndented, "Braces Indented"},
    {tree_sitter_format::Style::Indentation::BodyIndented,   "Body Indented"},
    {tree_sitter_format::Style::Indentation::BothIndented,   "Both Indented"},
    {tree_sitter_format::Style::Indentation::None,           "None"},
};

inline const std::unordered_map<tree_sitter_format::Style::BraceExistance, std::string> BRACE_EXISTANCE_NAMES = {
    {tree_sitter_format::Style::BraceExistance::Require, "Require"},
    {tree_sitter_format::Style::BraceExistance::Remove,  "Remove"},
    {tree_sitter_format::Style::BraceExistance::Ignore,  "Ignore"},
};

inline const std::unordered_map<tree_sitter_format::Style::SpacePlacement, std::string> SPACE_PLACEMENT_NAMES = {
    {tree_sitter_format::Style::SpacePlacement::None,     "None"},
    {tree_sitter_format::Style::SpacePlacement::Before,   "Before"},
    {tree_sitter_format::Style::SpacePlacement::After,    "After"},
    {tree_sitter_format::Style::SpacePlacement::Surround, "Surround"},
    {tree_sitter_format::Style::SpacePlacement::Ignore,   "Ignore"},
};

struct TestCase {
    std::string inputLabel;
    std::string inputValue;

    std::string outputValue;
};

template<typename STYLE_TYPE>
struct TestSuite {
    STYLE_TYPE style;
    std::vector<TestCase> cases;
};

template<typename STYLE_TYPE>
using StyleUpdater = void(tree_sitter_format::Style&, STYLE_TYPE);

template<typename STYLE_TYPE>
struct TestContext {
    const std::unordered_map<STYLE_TYPE, std::string>& styleNames;
    tree_sitter_format::Formatter formatter;
    tree_sitter_format::Style style;
    StyleUpdater<STYLE_TYPE>* styleUpdater;
};

template<typename STYLE_TYPE>
void RunTest(TestContext<STYLE_TYPE>& context, const std::vector<TestSuite<STYLE_TYPE>>& tests) {
    for(const TestSuite<STYLE_TYPE>& suite : tests) {
        SECTION(context.styleNames.at(suite.style)) {
            context.styleUpdater(context.style, suite.style);

            for(const TestCase& testCase : suite.cases) {
                SECTION(testCase.inputLabel) {
                    tree_sitter_format::Document document(testCase.inputValue);
                    context.formatter.format(context.style, document);

                    const std::string actual = document.toString();
                    const std::string expected = testCase.outputValue;

                    REQUIRE(actual == expected);
                }
            }
        }
    }
}

}
