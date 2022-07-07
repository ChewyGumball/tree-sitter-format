#pragma once

#include <cstdint>
#include <string>

namespace tree_sitter_format {


struct Style {
    enum class Whitespace { Spaces, Tabs };
    struct {
        Whitespace whitespace = Whitespace::Spaces;
        uint32_t indentationAmount = 4;
        uint32_t tabWidth = 4;
        bool reindent = true;
    } indentation;

    const std::string& indentationString() const;

    enum class LinePlacement { SameLine, NextLine, Ignore };
    struct {
        LinePlacement namespaceDefinitions = LinePlacement::SameLine;
        LinePlacement functionDeclarations = LinePlacement::SameLine;
        LinePlacement functionDefinitions = LinePlacement::SameLine;
        LinePlacement structDefinitions = LinePlacement::SameLine;
        LinePlacement classDefinitions = LinePlacement::SameLine;
        LinePlacement ifStatements = LinePlacement::SameLine;
        LinePlacement elseStatements = LinePlacement::SameLine;
        LinePlacement switchStatements = LinePlacement::SameLine;
        LinePlacement caseBlocks = LinePlacement::SameLine;
        LinePlacement whileLoops = LinePlacement::SameLine;
        LinePlacement forLoops = LinePlacement::SameLine;
    } bracePlacement;

    LinePlacement elseKeyword = LinePlacement::SameLine;

    enum class BraceExistance { Require, Remove, Ignore };
    struct {
        BraceExistance ifStatements = BraceExistance::Require;
        BraceExistance switchStatements = BraceExistance::Require;
        BraceExistance caseBlocks = BraceExistance::Require;
        BraceExistance whileLoops = BraceExistance::Require;
        BraceExistance forLoops = BraceExistance::Require;
    } braceExistance;

    enum class SpacePlacement { None, Before, After, Surround, Ignore };
    struct {
        struct {
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Surround;
        } namespaces;

        struct {
            SpacePlacement openingParenthesis = SpacePlacement::None;
            SpacePlacement closingParenthesis = SpacePlacement::After;
            SpacePlacement commas = SpacePlacement::After;
        } functionDeclarations;

        struct {
            SpacePlacement openingParenthesis = SpacePlacement::None;
            SpacePlacement closingParenthesis = SpacePlacement::After;
            SpacePlacement commas = SpacePlacement::After;
            SpacePlacement openingBrace = SpacePlacement::Surround;
        } functionDefinitions;

        struct {
            SpacePlacement openingParenthesis = SpacePlacement::None;
            SpacePlacement closingParenthesis = SpacePlacement::After;
            SpacePlacement commas = SpacePlacement::After;
        } functionCalls;

        struct {
            SpacePlacement forEachColon = SpacePlacement::Surround;
            SpacePlacement semicolons = SpacePlacement::After;
            SpacePlacement commas = SpacePlacement::After;
            SpacePlacement openingParenthesis = SpacePlacement::Before;
            SpacePlacement closingParenthesis = SpacePlacement::After;
            SpacePlacement openingBrace = SpacePlacement::Surround;
        } forLoops;

        struct {
            SpacePlacement openingParenthesis = SpacePlacement::Before;
            SpacePlacement closingParenthesis = SpacePlacement::After;
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement semicolons = SpacePlacement::After;
            SpacePlacement commas = SpacePlacement::After;
        } whileLoops;

        struct {
            SpacePlacement openingParenthesis = SpacePlacement::Before;
            SpacePlacement closingParenthesis = SpacePlacement::After;
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Surround;
            SpacePlacement semicolons = SpacePlacement::After;
            SpacePlacement commas = SpacePlacement::After;
        } doWhileLoops;

        struct {
            SpacePlacement openingParenthesis = SpacePlacement::Before;
            SpacePlacement closingParenthesis = SpacePlacement::After;
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Before;
            SpacePlacement elseKeyword = SpacePlacement::Surround;
            SpacePlacement semicolons = SpacePlacement::After;
            SpacePlacement commas = SpacePlacement::After;
        } ifStatements;

        struct {
            SpacePlacement openingParenthesis = SpacePlacement::Before;
            SpacePlacement closingParenthesis = SpacePlacement::After;
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Before;
        } switchStatements;

        struct {
            SpacePlacement colon = SpacePlacement::After;
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Before;
        } caseStatements;


        bool respace = true;
    } spacing;
};

}
