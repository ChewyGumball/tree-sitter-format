#include <tree_sitter_format/style/Style.h>

#include <yaml-cpp/yaml.h>

#include <iostream>

namespace {
    using namespace tree_sitter_format;
    std::string CreateIndentationString(const Style& style) {
        char c = style.indentation.whitespace == Style::IndentationWhitespace::Tabs ? '\t' : ' ';
        return std::string(style.indentation.indentationAmount, c);
    }

    std::string CreateNewLineString(const Style& style) {
        switch (style.spacing.newLineType) {
            case Style::NewLineType::CRLF: return "\r\n";
            case Style::NewLineType::LF: return "\n";
            case Style::NewLineType::CR: return "\r";
            default: return "\n";
        }
    }

    YAML::Node GetMap(YAML::Node node, const std::string& name) {
        YAML::Node map = node[name];
        if (!map.IsDefined()) {
            return YAML::Node(YAML::NodeType::Map);
        }

        if (!map.IsMap()) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a map, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return map;
    }

    Style::Indentation GetOptionalIndentation(YAML::Node node, const std::string& name, Style::Indentation defaultValue) {
        YAML::Node indentation = node[name];

        if (!indentation.IsDefined()) {
            return defaultValue;
        }

        if (!indentation.IsScalar()) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a scalar value, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string value = indentation.as<std::string>();

        if (value == "BracesIndented") {
            return Style::Indentation::BracesIndented;
        } else if (value == "BodyIndented") {
            return Style::Indentation::BodyIndented;
        } else if (value == "BothIndented") {
            return Style::Indentation::BothIndented;
        } else if (value == "None") {
            return Style::Indentation::None;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tBracesIndented, BodyIndented, BothIndented, None" << std::endl;
            std::cerr << "but it was: " << value << std::endl;

            std::exit(EXIT_FAILURE);
        }
    }

    Style::BraceExistance GetOptionalBraceExistance(YAML::Node node, const std::string& name, Style::BraceExistance defaultValue) {
        YAML::Node existance = node[name];

        if (!existance.IsDefined()) {
            return defaultValue;
        }

        if (!existance.IsScalar()) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a scalar value, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string value = existance.as<std::string>();

        if (value == "Require") {
            return Style::BraceExistance::Require;
        } else if (value == "Remove") {
            return Style::BraceExistance::Remove;
        } else if (value == "Ignore") {
            return Style::BraceExistance::Ignore;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tRequire, Remove, BothIndented, Ignore" << std::endl;
            std::cerr << "but it was: " << value << std::endl;

            std::exit(EXIT_FAILURE);
        }
    }

    Style::IndentationWhitespace GetOptionalIndentationWhitespace(YAML::Node node, const std::string& name, Style::IndentationWhitespace defaultValue) {
        YAML::Node whitespace = node[name];

        if (!whitespace.IsDefined()) {
            return defaultValue;
        }

        if (!whitespace.IsScalar()) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a scalar value, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string value = whitespace.as<std::string>();

        if (value == "Spaces") {
            return Style::IndentationWhitespace::Spaces;
        } else if (value == "Tabs") {
            return Style::IndentationWhitespace::Tabs;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tSpaces, Tags" << std::endl;
            std::cerr << "but it was: " << value << std::endl;

            std::exit(EXIT_FAILURE);
        }
    }

    Style::NewLineType GetOptionalNewLineType(YAML::Node node, const std::string& name, Style::NewLineType defaultValue) {
        YAML::Node newLineType = node[name];

        if (!newLineType.IsDefined()) {
            return defaultValue;
        }

        if (!newLineType.IsScalar()) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a scalar value, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string value = newLineType.as<std::string>();

        if (value == "CRLF") {
            return Style::NewLineType::CRLF;
        } else if (value == "LF") {
            return Style::NewLineType::LF;
        } else if (value == "CR") {
            return Style::NewLineType::CR;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tCRLF, LF, CR" << std::endl;
            std::cerr << "but it was: " << value << std::endl;

            std::exit(EXIT_FAILURE);
        }
    }

    Style::Whitespace GetOptionalWhitespace(YAML::Node node, const std::string& name, Style::Whitespace defaultValue) {
        YAML::Node whitespace = node[name];

        if (!whitespace.IsDefined()) {
            return defaultValue;
        }

        if (!whitespace.IsScalar()) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a scalar value, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string value = whitespace.as<std::string>();

        if (value == "Space") {
            return Style::Whitespace::Space;
        } else if (value == "Newline") {
            return Style::Whitespace::Newline;
        } else if (value == "Ignore") {
            return Style::Whitespace::Ignore;
        } else if (value == "None") {
            return Style::Whitespace::None;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tSpace, Newline, Ignore, None" << std::endl;
            std::cerr << "but it was: " << value << std::endl;

            std::exit(EXIT_FAILURE);
        }
    }

    Style::RequiredWhitespace GetOptionalRequiredWhitespace(YAML::Node node, const std::string& name, Style::RequiredWhitespace defaultValue) {
        YAML::Node whitespace = node[name];

        if (!whitespace.IsDefined()) {
            return defaultValue;
        }

        if (!whitespace.IsScalar()) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a scalar value, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string value = whitespace.as<std::string>();

        if (value == "Space") {
            return Style::RequiredWhitespace::Space;
        } else if (value == "Newline") {
            return Style::RequiredWhitespace::Newline;
        } else if (value == "Ignore") {
            return Style::RequiredWhitespace::Ignore;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tSpace, Newline, Ignore" << std::endl;
            std::cerr << "but it was: " << value << std::endl;

            std::exit(EXIT_FAILURE);
        }
    }

    uint32_t GetOptionalInt(YAML::Node node, const std::string& name, uint32_t defaultValue) {
        YAML::Node value = node[name];

        if (!value.IsDefined()) {
            return defaultValue;
        }

        try {
            return value.as<uint32_t>();
        } catch(YAML::TypedBadConversion<uint32_t>) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a non negative number, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    bool GetOptionalBoolean(YAML::Node node, const std::string& name, bool defaultValue) {
        YAML::Node value = node[name];

        if (!value.IsDefined()) {
            return defaultValue;
        }

        try {
            return value.as<bool>();
        } catch(YAML::TypedBadConversion<bool>) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a boolean, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    std::string GetOptionalString(YAML::Node node, const std::string& name, const std::string& defaultValue) {
        YAML::Node value = node[name];

        if (!value.IsDefined()) {
            return defaultValue;
        }

        if (!value.IsScalar()) {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a string, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return value.as<std::string>();
    }

    void SetIndentation(YAML::Node node, const std::string& name, Style::Indentation& indentation) {
        indentation = GetOptionalIndentation(node, name, indentation);
    }

    void SetInt(YAML::Node node, const std::string& name, uint32_t& integer) {
        integer = GetOptionalInt(node, name, integer);
    }

    void SetBool(YAML::Node node, const std::string& name, bool& boolean) {
        boolean = GetOptionalBoolean(node, name, boolean);
    }

    void SetIndentationWhitespace(YAML::Node node, const std::string& name, Style::IndentationWhitespace& whitespace) {
        whitespace = GetOptionalIndentationWhitespace(node, name, whitespace);
    }

    void GetBraceExistance(YAML::Node node, const std::string& name, Style::BraceExistance& brace) {
        brace = GetOptionalBraceExistance(node, name, brace);
    }

    void GetNewLineType(YAML::Node node, const std::string& name, Style::NewLineType& newLine) {
        newLine = GetOptionalNewLineType(node, name, newLine);
    }

    void GetWhitespace(YAML::Node node, const std::string& name, Style::Whitespace& whitespace) {
        whitespace = GetOptionalWhitespace(node, name, whitespace);
    }

    void GetWhitespacePlacement(YAML::Node node, const std::string& name, Style::WhitespacePlacement& placement) {
        YAML::Node placementYAML = GetMap(node, name);
        GetWhitespace(placementYAML, "before", placement.before);
        GetWhitespace(placementYAML, "after", placement.after);
    }

    void GetPairedWhitespace(YAML::Node node, const std::string& name, Style::PairedWhitespace& whitespace) {
        YAML::Node pair = GetMap(node, name);
        GetWhitespacePlacement(pair, "opening", whitespace.opening);
        GetWhitespacePlacement(pair, "closing", whitespace.closing);
    }

    void GetRequiredWhitespace(YAML::Node node, const std::string& name, Style::RequiredWhitespace& required) {
        required = GetOptionalRequiredWhitespace(node, name, required);
    }
}

namespace tree_sitter_format {
    std::string_view Style::indentationString() const {
        static std::string s = CreateIndentationString(*this); 
        return s;
    }
    std::string_view Style::newLineString() const {
        static std::string s = CreateNewLineString(*this); 
        return s;
    }
    
    Style Style::FromClangFormat(const std::string& config) {
        Style style;

        YAML::Node root = YAML::Load(config);

        // Based on https://releases.llvm.org/15.0.0/tools/clang/docs/ClangFormatStyleOptions.html

        // BasedOnStyle
        // AccessModifierOffset 
        // AlignAfterOpenBracket
        // AlignArrayOfStructures
        // AlignConsecutiveAssignments
        // AlignConsecutiveBitFields 
        // AlignConsecutiveDeclarations 
        // AlignConsecutiveMacros 
        // AlignEscapedNewlines 
        // AlignOperands 
        // AlignTrailingComments 
        // AllowAllArgumentsOnNextLine 
        // AllowAllConstructorInitializersOnNextLine 
        // AllowAllParametersOfDeclarationOnNextLine 
        // AllowShortBlocksOnASingleLine 
        // AllowShortCaseLabelsOnASingleLine 
        // AllowShortEnumsOnASingleLine 
        // AllowShortFunctionsOnASingleLine 
        // AllowShortIfStatementsOnASingleLine 
        // AllowShortLambdasOnASingleLine 
        // AllowShortLoopsOnASingleLine 

        // AlwaysBreakAfterDefinitionReturnType 
        std::string definitionReturnType = GetOptionalString(root, "AlwaysBreakAfterDefinitionReturnType ", "None");
        if (definitionReturnType == "All") {
            style.spacing.functionDefinitions.returnType = RequiredWhitespace::Newline;
        }

        // AlwaysBreakAfterReturnType 
        std::string returnType = GetOptionalString(root, "AlwaysBreakAfterReturnType ", "None");
        if (returnType == "All") {
            style.spacing.functionDeclarations.returnType = RequiredWhitespace::Newline;
            style.spacing.functionDefinitions.returnType = RequiredWhitespace::Newline;
        } else if (returnType == "AllDefinitions") {
            style.spacing.functionDefinitions.returnType = RequiredWhitespace::Newline;
        }

        // AlwaysBreakBeforeMultilineStrings 
        // AlwaysBreakTemplateDeclarations 
        // AttributeMacros 
        // BinPackArguments 
        // BinPackParameters 

        // BitFieldColonSpacing 
        std::string bitFields = GetOptionalString(root, "BitFieldColonSpacingStyle", "Both");
        if (bitFields == "Both") {
            style.spacing.bitFields.colon.before = Style::Whitespace::Space;
            style.spacing.bitFields.colon.after = Style::Whitespace::Space;
        } else if (bitFields == "None") {
            style.spacing.bitFields.colon.before = Style::Whitespace::None;
            style.spacing.bitFields.colon.after = Style::Whitespace::None;
        } else if (bitFields == "Before") {
            style.spacing.bitFields.colon.before = Style::Whitespace::Space;
            style.spacing.bitFields.colon.after = Style::Whitespace::None;
        } else if (bitFields == "After") {
            style.spacing.bitFields.colon.before = Style::Whitespace::None;
            style.spacing.bitFields.colon.after = Style::Whitespace::Space;
        } else {
            std::cerr << "Unknown value of 'BitFieldColonSpacingStyle': " << bitFields << std::endl;
            std::exit(EXIT_FAILURE);
        }


        // BreakAfterJavaFieldAnnotations 
        // BreakBeforeBinaryOperators 

        // BreakBeforeBraces
        std::string braceBreak = GetOptionalString(root, "BreakBeforeBraces", "Attach");
        if (braceBreak == "Attach") {
            style.spacing.namespaces.braces.opening.before = Whitespace::Space;
            style.spacing.classes.braces.opening.before = Whitespace::Space;
            style.spacing.structs.braces.opening.before = Whitespace::Space;
            style.spacing.unions.braces.opening.before = Whitespace::Space;
            style.spacing.enums.braces.opening.before = Whitespace::Space;
            style.spacing.functionDefinitions.braces.opening.before = Whitespace::Space;
            style.spacing.forLoops.braces.opening.before = Whitespace::Space;
            style.spacing.whileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.doWhileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.elseBraces.opening.before = Whitespace::Space;
            style.spacing.switchStatements.braces.opening.before = Whitespace::Space;
            style.spacing.caseStatements.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.catchBraces.opening.before = Whitespace::Space;
        } else if (braceBreak == "Linux") {
            style.spacing.structs.braces.opening.before = Whitespace::Space;
            style.spacing.unions.braces.opening.before = Whitespace::Space;
            style.spacing.enums.braces.opening.before = Whitespace::Space;
            style.spacing.forLoops.braces.opening.before = Whitespace::Space;
            style.spacing.whileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.doWhileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.elseBraces.opening.before = Whitespace::Space;
            style.spacing.switchStatements.braces.opening.before = Whitespace::Space;
            style.spacing.caseStatements.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.catchBraces.opening.before = Whitespace::Space;

            style.spacing.classes.braces.opening.before = Whitespace::Newline;
            style.spacing.functionDefinitions.braces.opening.before = Whitespace::Newline;
            style.spacing.namespaces.braces.opening.before = Whitespace::Newline;
        } else if (braceBreak == "Mozilla") {
            style.spacing.namespaces.braces.opening.before = Whitespace::Space;
            style.spacing.structs.braces.opening.before = Whitespace::Space;
            style.spacing.unions.braces.opening.before = Whitespace::Space;
            style.spacing.forLoops.braces.opening.before = Whitespace::Space;
            style.spacing.whileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.doWhileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.elseBraces.opening.before = Whitespace::Space;
            style.spacing.switchStatements.braces.opening.before = Whitespace::Space;
            style.spacing.caseStatements.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.catchBraces.opening.before = Whitespace::Space;

            style.spacing.enums.braces.opening.before = Whitespace::Newline;
            style.spacing.functionDefinitions.braces.opening.before = Whitespace::Newline;
            style.spacing.classes.braces.opening.before = Whitespace::Newline;
            style.spacing.structs.braces.opening.before = Whitespace::Newline;
        } else if (braceBreak == "Stroustrup") {
            style.spacing.namespaces.braces.opening.before = Whitespace::Space;
            style.spacing.classes.braces.opening.before = Whitespace::Space;
            style.spacing.structs.braces.opening.before = Whitespace::Space;
            style.spacing.unions.braces.opening.before = Whitespace::Space;
            style.spacing.enums.braces.opening.before = Whitespace::Space;
            style.spacing.forLoops.braces.opening.before = Whitespace::Space;
            style.spacing.whileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.doWhileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.braces.opening.before = Whitespace::Space;
            style.spacing.switchStatements.braces.opening.before = Whitespace::Space;
            style.spacing.caseStatements.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.braces.opening.before = Whitespace::Space;
            
            style.spacing.functionDefinitions.braces.opening.before = Whitespace::Newline;
            style.spacing.ifStatements.elseBraces.opening.before = Whitespace::Newline;
            style.spacing.tryCatch.catchBraces.opening.before = Whitespace::Newline;
        } else if (braceBreak == "Allman") {
            style.spacing.namespaces.braces.opening.before = Whitespace::Newline;
            style.spacing.classes.braces.opening.before = Whitespace::Newline;
            style.spacing.structs.braces.opening.before = Whitespace::Newline;
            style.spacing.unions.braces.opening.before = Whitespace::Newline;
            style.spacing.enums.braces.opening.before = Whitespace::Newline;
            style.spacing.functionDefinitions.braces.opening.before = Whitespace::Newline;
            style.spacing.forLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.whileLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.doWhileLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.ifStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.ifStatements.elseBraces.opening.before = Whitespace::Newline;
            style.spacing.switchStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.caseStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.tryCatch.braces.opening.before = Whitespace::Newline;
            style.spacing.tryCatch.catchBraces.opening.before = Whitespace::Newline;
        } else if (braceBreak == "Whitesmiths") {
            style.spacing.namespaces.braces.opening.before = Whitespace::Newline;
            style.spacing.classes.braces.opening.before = Whitespace::Newline;
            style.spacing.structs.braces.opening.before = Whitespace::Newline;
            style.spacing.unions.braces.opening.before = Whitespace::Newline;
            style.spacing.enums.braces.opening.before = Whitespace::Newline;
            style.spacing.functionDefinitions.braces.opening.before = Whitespace::Newline;
            style.spacing.forLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.whileLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.doWhileLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.ifStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.ifStatements.elseBraces.opening.before = Whitespace::Newline;
            style.spacing.switchStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.caseStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.tryCatch.braces.opening.before = Whitespace::Newline;
            style.spacing.tryCatch.catchBraces.opening.before = Whitespace::Newline;

            style.indentation.namespaces = Indentation::BracesIndented;
            style.indentation.functionDefinitions = Indentation::BracesIndented;
            style.indentation.structDefinitions = Indentation::BracesIndented;
            style.indentation.classDefinitions = Indentation::BracesIndented;
            style.indentation.ifStatements = Indentation::BracesIndented;
            style.indentation.switchStatements = Indentation::BracesIndented;
            style.indentation.caseBlocks = Indentation::BracesIndented;
            style.indentation.whileLoops = Indentation::BracesIndented;
            style.indentation.doWhileLoops = Indentation::BracesIndented;
            style.indentation.forLoops = Indentation::BracesIndented;
            style.indentation.genericScope = Indentation::BracesIndented;
            style.indentation.tryCatch = Indentation::BracesIndented;
        } else if (braceBreak == "GNU") {
            style.spacing.namespaces.braces.opening.before = Whitespace::Newline;
            style.spacing.classes.braces.opening.before = Whitespace::Newline;
            style.spacing.structs.braces.opening.before = Whitespace::Newline;
            style.spacing.unions.braces.opening.before = Whitespace::Newline;
            style.spacing.enums.braces.opening.before = Whitespace::Newline;
            style.spacing.functionDefinitions.braces.opening.before = Whitespace::Newline;
            style.spacing.forLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.whileLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.doWhileLoops.braces.opening.before = Whitespace::Newline;
            style.spacing.ifStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.ifStatements.elseBraces.opening.before = Whitespace::Newline;
            style.spacing.switchStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.caseStatements.braces.opening.before = Whitespace::Newline;
            style.spacing.tryCatch.braces.opening.before = Whitespace::Newline;
            style.spacing.tryCatch.catchBraces.opening.before = Whitespace::Newline;

            style.indentation.ifStatements = Indentation::BothIndented;
            style.indentation.switchStatements = Indentation::BothIndented;
            style.indentation.caseBlocks = Indentation::BothIndented;
            style.indentation.whileLoops = Indentation::BothIndented;
            style.indentation.doWhileLoops = Indentation::BothIndented;
            style.indentation.forLoops = Indentation::BothIndented;
            style.indentation.genericScope = Indentation::BothIndented;
            style.indentation.tryCatch = Indentation::BothIndented;
        } else if (braceBreak == "Webkit") {
            style.spacing.namespaces.braces.opening.before = Whitespace::Space;
            style.spacing.classes.braces.opening.before = Whitespace::Space;
            style.spacing.structs.braces.opening.before = Whitespace::Space;
            style.spacing.unions.braces.opening.before = Whitespace::Space;
            style.spacing.enums.braces.opening.before = Whitespace::Space;
            style.spacing.forLoops.braces.opening.before = Whitespace::Space;
            style.spacing.whileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.doWhileLoops.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.braces.opening.before = Whitespace::Space;
            style.spacing.ifStatements.elseBraces.opening.before = Whitespace::Space;
            style.spacing.switchStatements.braces.opening.before = Whitespace::Space;
            style.spacing.caseStatements.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.braces.opening.before = Whitespace::Space;
            style.spacing.tryCatch.catchBraces.opening.before = Whitespace::Space;

            style.spacing.functionDefinitions.braces.opening.before = Whitespace::Newline;
        } else if (braceBreak == "Custom") {
            // BraceWrapping 
            YAML::Node custom = GetMap(root, "BraceWrapping");

            // AfterCaseLabel 
            if (GetOptionalBoolean(custom, "AfterCaseLabel", false)) {
                style.spacing.caseStatements.braces.opening.before = Whitespace::Newline;
            } else {
                style.spacing.caseStatements.braces.opening.before = Whitespace::Space;
            }

            // AfterClass 
            if (GetOptionalBoolean(custom, "AfterClass", false)) {
                style.spacing.classes.braces.opening.before = Whitespace::Newline;
            } else {
                style.spacing.classes.braces.opening.before = Whitespace::Space;
            }

            // AfterControlStatement 

            // AfterEnum 
            if (GetOptionalBoolean(custom, "AfterEnum", false)) {
                style.spacing.enums.braces.opening.before = Whitespace::Newline;
            } else {
                style.spacing.enums.braces.opening.before = Whitespace::Space;
            }

            // AfterFunction  
            if (GetOptionalBoolean(custom, "AfterFunction ", false)) {
                style.spacing.functionDefinitions.braces.opening.before = Whitespace::Newline;
            } else {
                style.spacing.functionDefinitions.braces.opening.before = Whitespace::Space;
            }

            // AfterNamespace   
            if (GetOptionalBoolean(custom, "AfterNamespace", false)) {
                style.spacing.namespaces.braces.opening.before = Whitespace::Newline;
            } else {
                style.spacing.namespaces.braces.opening.before = Whitespace::Space;
            }

            // AfterObjCDeclaration 

            // AfterStruct    
            if (GetOptionalBoolean(custom, "AfterStruct ", false)) {
                style.spacing.structs.braces.opening.before = Whitespace::Newline;
            } else {
                style.spacing.structs.braces.opening.before = Whitespace::Space;
            }

            // AfterUnion     
            if (GetOptionalBoolean(custom, "AfterUnion  ", false)) {
                style.spacing.unions.braces.opening.before = Whitespace::Newline;
            } else {
                style.spacing.unions.braces.opening.before = Whitespace::Space;
            }

            // AfterExternBlock 

            // BeforeCatch      
            if (GetOptionalBoolean(custom, "BeforeCatch", false)) {
                style.spacing.tryCatch.braces.closing.before = Whitespace::Newline;
            } else {
                style.spacing.tryCatch.braces.closing.before = Whitespace::Space;
            }

            // BeforeLambdaBody 

            // BeforeWhile 
            if (GetOptionalBoolean(custom, "BeforeWhile", false)) {
                style.spacing.doWhileLoops.braces.closing.before = Whitespace::Newline;
            } else {
                style.spacing.doWhileLoops.braces.closing.before = Whitespace::Space;
            }

            // IndentBraces
            if (GetOptionalBoolean(custom, "IndentBraces", false)) {
                style.indentation.namespaces = Indentation::BracesIndented;
                style.indentation.functionDefinitions = Indentation::BracesIndented;
                style.indentation.structDefinitions = Indentation::BracesIndented;
                style.indentation.classDefinitions = Indentation::BracesIndented;
                style.indentation.ifStatements = Indentation::BracesIndented;
                style.indentation.switchStatements = Indentation::BracesIndented;
                style.indentation.caseBlocks = Indentation::BracesIndented;
                style.indentation.whileLoops = Indentation::BracesIndented;
                style.indentation.doWhileLoops = Indentation::BracesIndented;
                style.indentation.forLoops = Indentation::BracesIndented;
                style.indentation.genericScope = Indentation::BracesIndented;
                style.indentation.tryCatch = Indentation::BracesIndented;
            } else {
                style.indentation.namespaces = Indentation::BodyIndented;
                style.indentation.functionDefinitions = Indentation::BodyIndented;
                style.indentation.structDefinitions = Indentation::BodyIndented;
                style.indentation.classDefinitions = Indentation::BodyIndented;
                style.indentation.ifStatements = Indentation::BodyIndented;
                style.indentation.switchStatements = Indentation::BodyIndented;
                style.indentation.caseBlocks = Indentation::BodyIndented;
                style.indentation.whileLoops = Indentation::BodyIndented;
                style.indentation.doWhileLoops = Indentation::BodyIndented;
                style.indentation.forLoops = Indentation::BodyIndented;
                style.indentation.genericScope = Indentation::BodyIndented;
                style.indentation.tryCatch = Indentation::BodyIndented;
            }

            // SplitEmptyFunction 
            // SplitEmptyRecord 
            // SplitEmptyNamespace 
        } else {
            std::cerr << "Unknown value of 'BreakBeforeBraces': " << braceBreak << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // BreakBeforeConceptDeclarations 
        // BreakBeforeTernaryOperators 
        // BreakConstructorInitializers 
        // BreakInheritanceList 
        // BreakStringLiterals

        // ColumnLimit 
        style.targetLineLength = GetOptionalInt(root, "ColumnLimit", style.targetLineLength);

        // CommentPragmas 
        // CompactNamespaces 
        // ConstructorInitializerAllOnOneLineOrOnePerLine 
        // ConstructorInitializerIndentWidth 
        // ContinuationIndentWidth 
        // Cpp11BracedListStyle 
        // DeriveLineEnding 
        // DerivePointerAlignment 
        // DisableFormat 
        // EmptyLineAfterAccessModifier 
        // EmptyLineBeforeAccessModifier 
        // ExperimentalAutoDetectBinPacking 
        // FixNamespaceComments 
        // ForEachMacros 
        // IfMacros 
        // IncludeBlocks 
        // IncludeCategories 
        // IncludeIsMainRegex 
        // IncludeIsMainSourceRegex 
        // IndentAccessModifiers 

        // IndentCaseBlocks 
        if (GetOptionalBoolean(root, "IndentCaseBlocks", false)) {
            style.indentation.caseBlocks = Indentation::BothIndented;
        } else {
            style.indentation.caseBlocks = Indentation::BodyIndented;
        }

        // IndentCaseLabels 
        if (GetOptionalBoolean(root, "IndentCaseLabels",false)) {
            style.indentation.switchStatements = Indentation::BothIndented;
        } else {
            style.indentation.switchStatements = Indentation::BodyIndented;
        }

        // IndentExternBlock 
        // IndentGotoLabels 
        // IndentPPDirectives 
        // IndentRequiresClause 

        // IndentWidth 
        style.indentation.indentationAmount = GetOptionalInt(root, "IndentWidth", style.indentation.indentationAmount);

        // IndentWrappedFunctionNames 

        // InsertBraces 
        if (GetOptionalBoolean(root, "InsertBraces", false)) {
            style.braces.ifStatements = BraceExistance::Require;
            style.braces.forLoops = BraceExistance::Require;
            style.braces.whileLoops = BraceExistance::Require;
            style.braces.doWhileLoops = BraceExistance::Require;
            style.braces.caseStatements = BraceExistance::Require;
            style.braces.switchStatements = BraceExistance::Require;
        } else {
            style.braces.ifStatements = BraceExistance::Ignore;
            style.braces.forLoops = BraceExistance::Ignore;
            style.braces.whileLoops = BraceExistance::Ignore;
            style.braces.doWhileLoops = BraceExistance::Ignore;
            style.braces.caseStatements = BraceExistance::Ignore;
            style.braces.switchStatements = BraceExistance::Ignore;
        }

        // InsertTrailingCommas 
        // JavaImportGroups 
        // JavaScriptQuotes 
        // JavaScriptWrapImports 
        // KeepEmptyLinesAtTheStartOfBlocks 
        // LambdaBodyIndentation 
        // Language 
        // MacroBlockBegin 
        // MacroBlockEnd 
        // MaxEmptyLinesToKeep 
        // NamespaceIndentation 
        // NamespaceMacros 
        // ObjCBinPackProtocolList 
        // ObjCBlockIndentWidth 
        // ObjCBreakBeforeNestedBlockParam 
        // ObjCSpaceAfterProperty 
        // ObjCSpaceBeforeProtocolList 
        // PPIndentWidth 
        // PackConstructorInitializers 
        // PenaltyBreakAssignment 
        // PenaltyBreakBeforeFirstCallParameter 
        // PenaltyBreakComment 
        // PenaltyBreakFirstLessLess 
        // PenaltyBreakOpenParenthesis 
        // PenaltyBreakString 
        // PenaltyBreakTemplateDeclaration 
        // PenaltyExcessCharacter 
        // PenaltyIndentedWhitespace 
        // PenaltyReturnTypeOnItsOwnLine 

        // PointerAlignment 
        std::string pointerAlignment = GetOptionalString(root, "PointerAlignment", "Left");
        if (pointerAlignment == "Left") {
            style.spacing.pointers.before = Whitespace::None;
            style.spacing.pointers.after = Whitespace::Space;
        } else if (pointerAlignment == "Right") {
            style.spacing.pointers.before = Whitespace::Space;
            style.spacing.pointers.after = Whitespace::None;
        } else if (pointerAlignment == "Middle") {
            style.spacing.pointers.before = Whitespace::Space;
            style.spacing.pointers.after = Whitespace::Space;
        } else {
            std::cerr << "Unknown value of 'PointerAlignment': " << pointerAlignment << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // QualifierAlignment 
        // QualifierOrder 
        // RawStringFormats 

        // ReferenceAlignment 
        std::string referenceAlignment = GetOptionalString(root, "ReferenceAlignment", "Left");
        if (referenceAlignment == "Left") {
            style.spacing.references.before = Whitespace::None;
            style.spacing.references.after = Whitespace::Space;
        } else if (referenceAlignment == "Right") {
            style.spacing.references.before = Whitespace::Space;
            style.spacing.references.after = Whitespace::None;
        } else if (referenceAlignment == "Middle") {
            style.spacing.references.before = Whitespace::Space;
            style.spacing.references.after = Whitespace::Space;
        } else {
            std::cerr << "Unknown value of 'ReferenceAlignment': " << referenceAlignment << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // ReflowComments 

        // RemoveBracesLLVM 
        if (GetOptionalBoolean(root, "RemoveBracesLLVM", false)) {
            style.braces.ifStatements = BraceExistance::Remove;
            style.braces.forLoops = BraceExistance::Remove;
            style.braces.whileLoops = BraceExistance::Remove;
            style.braces.doWhileLoops = BraceExistance::Remove;
        }

        // RequiresClausePosition 
        // SeparateDefinitionBlocks 
        // ShortNamespaceLines 
        // SortIncludes 
        // SortJavaStaticImport 
        // SortUsingDeclarations 
        // SpaceAfterCStyleCast 
        // SpaceAfterLogicalNot 
        // SpaceAfterTemplateKeyword 
        // SpaceAroundPointerQualifiers 
        // SpaceBeforeAssignmentOperators 

        // SpaceBeforeCaseColon 
        if (GetOptionalBoolean(root, "SpaceBeforeCaseColon", false)) {
            style.spacing.caseStatements.colon.before = Whitespace::Space;
        } else {
            style.spacing.caseStatements.colon.before = Whitespace::None;
        }

        // SpaceBeforeCpp11BracedList 
        // SpaceBeforeCtorInitializerColon 
        // SpaceBeforeInheritanceColon 

        // SpaceBeforeParens 
        std::string spaceBefore = GetOptionalString(root, "SpaceBeforeParens", "Always");
        if (spaceBefore == "Never") {
            style.spacing.functionDeclarations.parentheses.opening.before = Whitespace::None;
            style.spacing.functionDefinitions.parentheses.opening.before = Whitespace::None;
            style.spacing.functionCalls.parentheses.opening.before = Whitespace::None;
            style.spacing.forLoops.parentheses.opening.before = Whitespace::None;
            style.spacing.whileLoops.parentheses.opening.before = Whitespace::None;
            style.spacing.doWhileLoops.parentheses.opening.before = Whitespace::None;
            style.spacing.ifStatements.parentheses.opening.before = Whitespace::None;
            style.spacing.switchStatements.parentheses.opening.before = Whitespace::None;
        } else if (spaceBefore == "ControlStatements") {
            style.spacing.forLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.whileLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.doWhileLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.ifStatements.parentheses.opening.before = Whitespace::Space;
            style.spacing.switchStatements.parentheses.opening.before = Whitespace::Space;
        } else if (spaceBefore == "ControlStatementsExceptControlMacros") {
            style.spacing.forLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.whileLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.doWhileLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.ifStatements.parentheses.opening.before = Whitespace::Space;
            style.spacing.switchStatements.parentheses.opening.before = Whitespace::Space;
        } else if (spaceBefore == "NonEmptyParentheses") {

        } else if (spaceBefore == "Always") {
            style.spacing.functionDeclarations.parentheses.opening.before = Whitespace::Space;
            style.spacing.functionDefinitions.parentheses.opening.before = Whitespace::Space;
            style.spacing.functionCalls.parentheses.opening.before = Whitespace::Space;
            style.spacing.forLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.whileLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.doWhileLoops.parentheses.opening.before = Whitespace::Space;
            style.spacing.ifStatements.parentheses.opening.before = Whitespace::Space;
            style.spacing.switchStatements.parentheses.opening.before = Whitespace::Space;
        } else if (spaceBefore == "Custom") {
            // SpaceBeforeParensOptions 
            YAML::Node custom = GetMap(root, "SpaceBeforeParensOptions");

            // AfterControlStatements 
            if (GetOptionalBoolean(custom, "AfterControlStatements", false)) {
                style.spacing.forLoops.parentheses.opening.before = Whitespace::Space;
                style.spacing.whileLoops.parentheses.opening.before = Whitespace::Space;
                style.spacing.doWhileLoops.parentheses.opening.before = Whitespace::Space;
                style.spacing.ifStatements.parentheses.opening.before = Whitespace::Space;
                style.spacing.switchStatements.parentheses.opening.before = Whitespace::Space;
            }

            // AfterForeachMacros 

            // AfterFunctionDeclarationName
            if (GetOptionalBoolean(custom, "AfterFunctionDeclarationName", false)) {
                style.spacing.functionDeclarations.parentheses.opening.before = Whitespace::Space;
            }
            
            // AfterFunctionDefinitionName 
            if (GetOptionalBoolean(custom, "AfterFunctionDefinitionName", false)) {
                style.spacing.functionDefinitions.parentheses.opening.before = Whitespace::Space;
            }
            
            // AfterIfMacros 
            // AfterOverloadedOperator 
            // AfterRequiresInClause 
            // AfterRequiresInExpression 
            // BeforeNonEmptyParentheses 
        } else {
            std::cerr << "Unknown value of 'SpaceBeforeParens': " << spaceBefore << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // SpaceBeforeRangeBasedForLoopColon 
        if (GetOptionalBoolean(root, "SpaceBeforeRangeBasedForLoopColon", false)) {
            style.spacing.forLoops.forEachColon.before = Whitespace::Space;
        }

        // SpaceBeforeSquareBrackets 
        // SpaceInEmptyBlock 
        // SpaceInEmptyParentheses 
        // SpacesBeforeTrailingComments 
        // SpacesInAngles 
        // SpacesInCStyleCastParentheses 

        // SpacesInConditionalStatement 
        if (GetOptionalBoolean(root, "SpacesInConditionalStatement", false)) {
            style.spacing.forLoops.parentheses.opening.after = Whitespace::Space;
            style.spacing.forLoops.parentheses.closing.before = Whitespace::Space;
            style.spacing.whileLoops.parentheses.opening.after = Whitespace::Space;
            style.spacing.whileLoops.parentheses.closing.before = Whitespace::Space;
            style.spacing.doWhileLoops.parentheses.opening.after = Whitespace::Space;
            style.spacing.doWhileLoops.parentheses.closing.before = Whitespace::Space;
            style.spacing.ifStatements.parentheses.opening.after = Whitespace::Space;
            style.spacing.ifStatements.parentheses.closing.before = Whitespace::Space;
            style.spacing.switchStatements.parentheses.opening.after = Whitespace::Space;
            style.spacing.switchStatements.parentheses.closing.before = Whitespace::Space;
        } else {
            style.spacing.forLoops.parentheses.opening.after = Whitespace::None;
            style.spacing.forLoops.parentheses.closing.before = Whitespace::None;
            style.spacing.whileLoops.parentheses.opening.after = Whitespace::None;
            style.spacing.whileLoops.parentheses.closing.before = Whitespace::None;
            style.spacing.doWhileLoops.parentheses.opening.after = Whitespace::None;
            style.spacing.doWhileLoops.parentheses.closing.before = Whitespace::None;
            style.spacing.ifStatements.parentheses.opening.after = Whitespace::None;
            style.spacing.ifStatements.parentheses.closing.before = Whitespace::None;
            style.spacing.switchStatements.parentheses.opening.after = Whitespace::None;
            style.spacing.switchStatements.parentheses.closing.before = Whitespace::None;
        }

        // SpacesInContainerLiterals 
        // SpacesInLineCommentPrefix 
        
        // SpacesInParentheses 
        if (GetOptionalBoolean(root, "SpacesInParentheses", false)) {
            style.spacing.functionDeclarations.parentheses.opening.after = Whitespace::Space;
            style.spacing.functionDeclarations.parentheses.closing.before = Whitespace::Space;
            style.spacing.functionDefinitions.parentheses.opening.after = Whitespace::Space;
            style.spacing.functionDefinitions.parentheses.closing.before = Whitespace::Space;
            style.spacing.functionCalls.parentheses.opening.after = Whitespace::Space;
            style.spacing.functionCalls.parentheses.closing.before = Whitespace::Space;
            style.spacing.parentheses.opening.after = Whitespace::Space;
            style.spacing.parentheses.closing.before = Whitespace::Space;
        } else {
            style.spacing.functionDeclarations.parentheses.opening.after = Whitespace::None;
            style.spacing.functionDeclarations.parentheses.closing.before = Whitespace::None;
            style.spacing.functionDefinitions.parentheses.opening.after = Whitespace::None;
            style.spacing.functionDefinitions.parentheses.closing.before = Whitespace::None;
            style.spacing.functionCalls.parentheses.opening.after = Whitespace::None;
            style.spacing.functionCalls.parentheses.closing.before = Whitespace::None;
            style.spacing.parentheses.opening.after = Whitespace::None;
            style.spacing.parentheses.closing.before = Whitespace::None;
        }

        // SpacesInSquareBrackets 
        // Standard 
        // StatementAttributeLikeMacros 
        // StatementMacros 

        // TabWidth 
        style.indentation.tabWidth = GetOptionalInt(root, "TabWidth", style.indentation.tabWidth);

        // TypenameMacros 

        // UseCRLF         
        if (GetOptionalBoolean(root, "UseCLRF", false)) {
            style.spacing.newLineType = NewLineType::CRLF;
        } else {
            style.spacing.newLineType = NewLineType::CR;
        }

        // UseTab 
        if (GetOptionalBoolean(root, "UseTab", false)) {
            style.indentation.whitespace = IndentationWhitespace::Tabs;
        } else {
            style.indentation.whitespace = IndentationWhitespace::Spaces;
        }

        // WhitespaceSensitiveMacros 

        return style;
    }

    Style Style::FromTreeSitterFormat(const std::string& config) {
        Style style;
        
        YAML::Node root = YAML::Load(config);

        YAML::Node indentation = GetMap(root, "indentation");
        SetIndentation(indentation, "namespaces", style.indentation.namespaces);
        SetIndentation(indentation, "function_definitions", style.indentation.functionDefinitions);
        SetIndentation(indentation, "struct_definitions", style.indentation.structDefinitions);
        SetIndentation(indentation, "class_definitions", style.indentation.classDefinitions);
        SetIndentation(indentation, "if_statements", style.indentation.ifStatements);
        SetIndentation(indentation, "switch_statements", style.indentation.switchStatements);
        SetIndentation(indentation, "case_blocks", style.indentation.caseBlocks);
        SetIndentation(indentation, "while_loops", style.indentation.whileLoops);
        SetIndentation(indentation, "do_while_loops", style.indentation.doWhileLoops);
        SetIndentation(indentation, "for_loops", style.indentation.forLoops);
        SetIndentation(indentation, "generic_scope", style.indentation.genericScope);
        SetIndentation(indentation, "try_catch", style.indentation.tryCatch);

        SetIndentationWhitespace(indentation, "whitespace", style.indentation.whitespace);
        SetInt(indentation, "indentation_amount", style.indentation.indentationAmount);
        SetInt(indentation, "tab_width", style.indentation.tabWidth);
        SetBool(indentation, "reindent", style.indentation.reindent);


        YAML::Node braces = GetMap(root, "braces");
        GetBraceExistance(braces, "if_statements", style.braces.ifStatements);
        GetBraceExistance(braces, "for_loops", style.braces.forLoops);
        GetBraceExistance(braces, "while_loops", style.braces.whileLoops);
        GetBraceExistance(braces, "do_while_loops", style.braces.doWhileLoops);
        GetBraceExistance(braces, "case_statements", style.braces.caseStatements);
        GetBraceExistance(braces, "switch_statements", style.braces.switchStatements);


        YAML::Node spacing = GetMap(root, "spacing");

        YAML::Node namespaces = GetMap(spacing, "namespaces");
        GetPairedWhitespace(namespaces, "braces", style.spacing.namespaces.braces);

        YAML::Node classes = GetMap(spacing, "classes");
        GetPairedWhitespace(classes, "braces", style.spacing.classes.braces);
        GetWhitespacePlacement(classes, "name", style.spacing.classes.name);

        YAML::Node structs = GetMap(spacing, "structs");
        GetPairedWhitespace(structs, "braces", style.spacing.structs.braces);
        GetWhitespacePlacement(structs, "name", style.spacing.structs.name);

        YAML::Node unions = GetMap(spacing, "unions");
        GetPairedWhitespace(unions, "braces", style.spacing.unions.braces);
        GetWhitespacePlacement(unions, "name", style.spacing.unions.name);

        YAML::Node enums = GetMap(spacing, "enums");
        GetPairedWhitespace(enums, "braces", style.spacing.enums.braces);
        GetWhitespacePlacement(enums, "name", style.spacing.enums.name);

        YAML::Node functionDeclarations = GetMap(spacing, "function_declarations");
        GetPairedWhitespace(functionDeclarations, "parentheses", style.spacing.functionDeclarations.parentheses);
        GetWhitespacePlacement(functionDeclarations, "commas", style.spacing.functionDeclarations.commas);
        GetRequiredWhitespace(functionDeclarations, "return_type", style.spacing.functionDeclarations.returnType);

        YAML::Node functionDefinitions = GetMap(spacing, "function_definitions");
        GetPairedWhitespace(functionDefinitions, "parentheses", style.spacing.functionDefinitions.parentheses);
        GetWhitespacePlacement(functionDefinitions, "commas", style.spacing.functionDefinitions.commas);
        GetPairedWhitespace(functionDefinitions, "braces", style.spacing.functionDefinitions.braces);
        GetRequiredWhitespace(functionDefinitions, "return_type", style.spacing.functionDefinitions.returnType);

        YAML::Node functionCalls = GetMap(spacing, "function_calls");
        GetPairedWhitespace(functionCalls, "parentheses", style.spacing.functionCalls.parentheses);
        GetWhitespacePlacement(functionCalls, "commas", style.spacing.functionCalls.commas);

        YAML::Node forLoops = GetMap(spacing, "for_loops");
        GetPairedWhitespace(forLoops, "parentheses", style.spacing.forLoops.parentheses);
        GetWhitespacePlacement(forLoops, "commas", style.spacing.forLoops.commas);
        GetWhitespacePlacement(forLoops, "semicolons", style.spacing.forLoops.semicolons);
        GetWhitespacePlacement(forLoops, "for_each_colon", style.spacing.forLoops.forEachColon);
        GetPairedWhitespace(forLoops, "braces", style.spacing.forLoops.braces);

        YAML::Node whileLoops = GetMap(spacing, "while_loops");
        GetPairedWhitespace(whileLoops, "parentheses", style.spacing.whileLoops.parentheses);
        GetWhitespacePlacement(whileLoops, "commas", style.spacing.whileLoops.commas);
        GetWhitespacePlacement(whileLoops, "semicolons", style.spacing.whileLoops.semicolons);
        GetPairedWhitespace(whileLoops, "braces", style.spacing.whileLoops.braces);

        YAML::Node doWhileLoops = GetMap(spacing, "do_while_loops");
        GetPairedWhitespace(doWhileLoops, "parentheses", style.spacing.doWhileLoops.parentheses);
        GetWhitespacePlacement(doWhileLoops, "commas", style.spacing.doWhileLoops.commas);
        GetWhitespacePlacement(doWhileLoops, "semicolons", style.spacing.doWhileLoops.semicolons);
        GetPairedWhitespace(doWhileLoops, "braces", style.spacing.doWhileLoops.braces);

        YAML::Node ifStatements = GetMap(spacing, "if_statements");
        GetPairedWhitespace(ifStatements, "parentheses", style.spacing.ifStatements.parentheses);
        GetWhitespacePlacement(ifStatements, "commas", style.spacing.ifStatements.commas);
        GetWhitespacePlacement(ifStatements, "semicolons", style.spacing.ifStatements.semicolons);
        GetPairedWhitespace(ifStatements, "braces", style.spacing.ifStatements.braces);
        GetPairedWhitespace(ifStatements, "else_braces", style.spacing.ifStatements.elseBraces);

        YAML::Node switchStatements = GetMap(spacing, "switch_statements");
        GetPairedWhitespace(switchStatements, "parentheses", style.spacing.switchStatements.parentheses);
        GetPairedWhitespace(switchStatements, "braces", style.spacing.switchStatements.braces);

        YAML::Node caseStatements = GetMap(spacing, "switch_statements");
        GetPairedWhitespace(caseStatements, "braces", style.spacing.caseStatements.braces);
        GetWhitespacePlacement(caseStatements, "colon", style.spacing.caseStatements.colon);

        YAML::Node bitFields = GetMap(spacing, "bitFields");
        GetWhitespacePlacement(bitFields, "colon", style.spacing.bitFields.colon);

        YAML::Node tryCatch = GetMap(spacing, "try_catch");
        GetPairedWhitespace(tryCatch, "braces", style.spacing.tryCatch.braces);
        GetPairedWhitespace(tryCatch, "catch_braces", style.spacing.tryCatch.catchBraces);

        GetPairedWhitespace(spacing, "parentheses", style.spacing.parentheses);
        GetWhitespacePlacement(spacing, "binary_operator", style.spacing.binaryOperator);
        GetWhitespacePlacement(spacing, "pointers", style.spacing.pointers);
        GetWhitespacePlacement(spacing, "references", style.spacing.references);

        GetNewLineType(spacing, "new_line_type", style.spacing.newLineType);
        SetBool(spacing, "respace", style.spacing.respace);
        SetBool(spacing, "trim_trailing", style.spacing.trimTrailing);

        return style;
    }
}
