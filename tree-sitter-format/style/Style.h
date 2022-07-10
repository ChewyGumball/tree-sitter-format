#pragma once

#include <cstdint>
#include <string>

namespace tree_sitter_format {


struct Style {
    enum class Whitespace { Spaces, Tabs };
    enum class Indentation { BracesIndented, BodyIndented, BothIndented, None };

    struct {
        Indentation namespaces = Indentation::None;
        Indentation functionDefinitions = Indentation::BodyIndented;
        Indentation structDefinitions = Indentation::BodyIndented;
        Indentation classDefinitions = Indentation::BodyIndented;
        Indentation ifStatements = Indentation::BodyIndented;
        Indentation switchStatements = Indentation::BodyIndented;
        Indentation caseBlocks = Indentation::BodyIndented;
        Indentation whileLoops = Indentation::BodyIndented;
        Indentation doWhileLoops = Indentation::BodyIndented;
        Indentation forLoops = Indentation::BodyIndented;

        Whitespace whitespace = Whitespace::Spaces;
        uint32_t indentationAmount = 4;
        uint32_t tabWidth = 4;
        bool reindent = true;
    } indentation;

    std::string_view indentationString() const;

    enum class BracePlacement { Before, After, Surround, Ignore };
    struct {
        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } namespaces;

        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } classes;
        
        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } structs;
        
        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } unions;

        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } enums;

        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } functionDefinitions;

        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } forLoops;

        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } whileLoops;

        struct {
            BracePlacement openingBrace = BracePlacement::Surround;
            BracePlacement closingBrace = BracePlacement::Before;
        } doWhileLoops;

        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } ifStatements;

        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } switchStatements;

        struct {
            BracePlacement openingBrace = BracePlacement::After;
            BracePlacement closingBrace = BracePlacement::Surround;
        } caseStatements;
    } bracePlacement;

    enum class LinePlacement { SameLine, NextLine, Ignore };
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
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Before;
        } classes;
        
        struct {
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Before;
            SpacePlacement name = SpacePlacement::Before;
        } structs;
        
        struct {
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Before;
            SpacePlacement name = SpacePlacement::Before;
        } unions;

        struct {
            SpacePlacement openingBrace = SpacePlacement::Surround;
            SpacePlacement closingBrace = SpacePlacement::Before;
            SpacePlacement commas = SpacePlacement::After;
        } enums;

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
