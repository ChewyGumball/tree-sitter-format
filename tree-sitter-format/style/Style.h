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

    enum class BracePlacement { SameLine, NextLine, Ignore };
    struct {
        BracePlacement namespaceDefinitions = BracePlacement::SameLine;
        BracePlacement functionDeclarations = BracePlacement::SameLine;
        BracePlacement functionDefinitions = BracePlacement::SameLine;
        BracePlacement structDefinitions = BracePlacement::SameLine;
        BracePlacement classDefinitions = BracePlacement::SameLine;
        BracePlacement ifStatements = BracePlacement::SameLine;
        BracePlacement switchCases = BracePlacement::SameLine;
        BracePlacement whileLoops = BracePlacement::SameLine;
        BracePlacement forLoops = BracePlacement::SameLine;
    } bracePlacement;

    enum class BraceExistance { Require, Remove, Ignore };
    struct {
        BraceExistance ifStatements = BraceExistance::Require;
        BraceExistance switchCases = BraceExistance::Require;
        BraceExistance whileLoops = BraceExistance::Require;
        BraceExistance forLoops = BraceExistance::Require;
    } braceExistance;

    const std::string& indentationString() const;
};

}
