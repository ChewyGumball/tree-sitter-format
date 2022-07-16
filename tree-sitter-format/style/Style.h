#pragma once

#include <cstdint>
#include <string>

namespace tree_sitter_format {

struct BraceStyle {

};


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
        Indentation genericScope = Indentation::BodyIndented;

        Whitespace whitespace = Whitespace::Spaces;
        uint32_t indentationAmount = 4;
        uint32_t tabWidth = 4;
        bool reindent = true;
    } indentation;

    std::string_view indentationString() const;

    enum class NewlinePlacement { Before, After, Surround, Ignore };
    enum class BraceExistance { Require, Remove, Ignore };
    struct Braces {
        NewlinePlacement openingBrace;
        NewlinePlacement closingBrace;
        BraceExistance existance;
    };

    struct {
        // Existance is ALWAYS required for these ones and shouldn't be modified
        Braces namespaces = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };

        Braces classes = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        Braces structs = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };

        Braces unions = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        Braces enums = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        Braces functionDefinitions = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        Braces switchStatements = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        // Existance is optional for these ones
        Braces forLoops = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        Braces whileLoops = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        Braces doWhileLoops = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        Braces ifStatements = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
        
        Braces caseStatements = {
            .openingBrace = NewlinePlacement::After,
            .closingBrace = NewlinePlacement::Surround,
            .existance = BraceExistance::Require,
        };
    } braces;

    enum class LinePlacement { SameLine, NextLine, Ignore };
    LinePlacement elseKeyword = LinePlacement::SameLine;

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
        bool trimTrailing = true;
    } spacing;
};

}
