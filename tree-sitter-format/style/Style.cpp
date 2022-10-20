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

        if (value == "braces_indented") {
            return Style::Indentation::BracesIndented;
        } else if (value == "body_indented") {
            return Style::Indentation::BodyIndented;
        } else if (value == "both_indented") {
            return Style::Indentation::BothIndented;
        } else if (value == "none") {
            return Style::Indentation::None;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\braces_indented, body_indented, both_indented, none" << std::endl;
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

        if (value == "require") {
            return Style::BraceExistance::Require;
        } else if (value == "remove") {
            return Style::BraceExistance::Remove;
        } else if (value == "ignore") {
            return Style::BraceExistance::Ignore;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\trequire, remove, ignore" << std::endl;
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

        if (value == "spaces") {
            return Style::IndentationWhitespace::Spaces;
        } else if (value == "tabs") {
            return Style::IndentationWhitespace::Tabs;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tspaces, tabs" << std::endl;
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

        if (value == "crlf") {
            return Style::NewLineType::CRLF;
        } else if (value == "lf") {
            return Style::NewLineType::LF;
        } else if (value == "cr") {
            return Style::NewLineType::CR;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tcrlf, lf, cr" << std::endl;
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

        if (value == "space") {
            return Style::Whitespace::Space;
        } else if (value == "new_line") {
            return Style::Whitespace::Newline;
        } else if (value == "ignore") {
            return Style::Whitespace::Ignore;
        } else if (value == "none") {
            return Style::Whitespace::None;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tspace, new_line, ignore, none" << std::endl;
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

        if (value == "space") {
            return Style::RequiredWhitespace::Space;
        } else if (value == "new_line") {
            return Style::RequiredWhitespace::Newline;
        } else if (value == "ignore") {
            return Style::RequiredWhitespace::Ignore;
        } else {
            YAML::Mark mark = node.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to have one of the following values:" << std::endl;
            std::cerr << "\tspace, new_line, ignore" << std::endl;
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

    void SetAlignmentClangFormat(YAML::Node node, const std::string& name, Style::Alignment& alignment) {
        YAML::Node assignments = node[name];
        if (assignments.IsMap()) {
            alignment.align = GetOptionalBoolean(assignments, "Enabled", true);
            alignment.acrossEmptyLines = GetOptionalBoolean(assignments, "AcrossEmptyLines", false);
            alignment.acrossComments = GetOptionalBoolean(assignments, "AcrossComments", false);
        } else if (assignments.IsScalar()) {
            try {
                std::string value = assignments.as<std::string>();
                
                alignment.align = value != "None";
                alignment.acrossEmptyLines = value == "AcrossEmptyLines" || value == "AcrossEmptyLinesAndComments";
                alignment.acrossComments = value == "AcrossComments" || value == "AcrossEmptyLinesAndComments";
            } catch(YAML::TypedBadConversion<bool>) {
                YAML::Mark mark = assignments.Mark();
                std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a string, but it wasn't!" << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            YAML::Mark mark = assignments.Mark();
            std::cerr << "Expected field '" << name << "' on line " << mark.line << " to be a string or a map, but it wasn't!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    void SetBraceExistance(YAML::Node node, const std::string& name, Style::BraceExistance& brace) {
        brace = GetOptionalBraceExistance(node, name, brace);
    }

    void SetNewLineType(YAML::Node node, const std::string& name, Style::NewLineType& newLine) {
        newLine = GetOptionalNewLineType(node, name, newLine);
    }

    void GetWhitespace(YAML::Node node, const std::string& name, Style::Whitespace& whitespace) {
        whitespace = GetOptionalWhitespace(node, name, whitespace);
    }

    void SetWhitespacePlacement(YAML::Node node, const std::string& name, Style::WhitespacePlacement& placement) {
        YAML::Node placementYAML = GetMap(node, name);
        GetWhitespace(placementYAML, "before", placement.before);
        GetWhitespace(placementYAML, "after", placement.after);
    }

    void SetPairedWhitespace(YAML::Node node, const std::string& name, Style::PairedWhitespace& whitespace) {
        YAML::Node pair = GetMap(node, name);
        SetWhitespacePlacement(pair, "opening", whitespace.opening);
        SetWhitespacePlacement(pair, "closing", whitespace.closing);
    }

    void SetRequiredWhitespace(YAML::Node node, const std::string& name, Style::RequiredWhitespace& required) {
        required = GetOptionalRequiredWhitespace(node, name, required);
    }

    void SetAlignment(YAML::Node node, const std::string& name, Style::Alignment& alignment) {
        YAML::Node alignmentYAML = GetMap(node, name);
        SetBool(alignmentYAML, "align", alignment.align);
        SetBool(alignmentYAML, "across_comments", alignment.acrossComments);
        SetBool(alignmentYAML, "across_empty_lines", alignment.acrossEmptyLines);
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
        std::string alignInitializers = GetOptionalString(root, "AlignArrayOfStructures", "None");
        style.alignment.initializerLists.alignment.acrossComments = false;
        style.alignment.initializerLists.alignment.acrossEmptyLines = false;

        if (alignInitializers == "None") {
            style.alignment.initializerLists.alignment.align = false;
            style.alignment.initializerLists.justification = Style::Justify::Left;
        } else if (alignInitializers == "Left") {
            style.alignment.initializerLists.alignment.align = true;
            style.alignment.initializerLists.alignCommasSeparately = false;
            style.alignment.initializerLists.justification = Style::Justify::Left;
        } else if (alignInitializers == "Right") {
            style.alignment.initializerLists.alignment.align = true;
            style.alignment.initializerLists.alignCommasSeparately = true;
            style.alignment.initializerLists.justification = Style::Justify::Right;
        } else {
            std::cerr << "Unknown value of 'AlignArrayOfStructures': " << alignInitializers << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // AlignConsecutiveAssignments
        SetAlignmentClangFormat(root, "AlignConsecutiveAssignments", style.alignment.assignments);

        // AlignConsecutiveBitFields 
        SetAlignmentClangFormat(root, "AlignConsecutiveBitFields", style.alignment.bitFields);

        // AlignConsecutiveDeclarations 
        SetAlignmentClangFormat(root, "AlignConsecutiveDeclarations", style.alignment.memberVariableDeclarations);
        SetAlignmentClangFormat(root, "AlignConsecutiveDeclarations", style.alignment.variableDeclarations);

        // AlignConsecutiveMacros 
        SetAlignmentClangFormat(root, "AlignConsecutiveMacros", style.alignment.macros);

        // AlignEscapedNewlines 
        SetAlignmentClangFormat(root, "AlignEscapedNewlines", style.alignment.escapedNewlines);

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
        std::string definitionReturnType = GetOptionalString(root, "AlwaysBreakAfterDefinitionReturnType", "None");
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

        //
        // Indentation
        //
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

        //
        // Braces
        //
        YAML::Node braces = GetMap(root, "braces");
        SetBraceExistance(braces, "if_statements", style.braces.ifStatements);
        SetBraceExistance(braces, "for_loops", style.braces.forLoops);
        SetBraceExistance(braces, "while_loops", style.braces.whileLoops);
        SetBraceExistance(braces, "do_while_loops", style.braces.doWhileLoops);
        SetBraceExistance(braces, "case_statements", style.braces.caseStatements);
        SetBraceExistance(braces, "switch_statements", style.braces.switchStatements);

        //
        // Spacing
        //
        YAML::Node spacing = GetMap(root, "spacing");

        YAML::Node namespaces = GetMap(spacing, "namespaces");
        SetPairedWhitespace(namespaces, "braces", style.spacing.namespaces.braces);

        YAML::Node classes = GetMap(spacing, "classes");
        SetPairedWhitespace(classes, "braces", style.spacing.classes.braces);
        SetWhitespacePlacement(classes, "name", style.spacing.classes.name);

        YAML::Node structs = GetMap(spacing, "structs");
        SetPairedWhitespace(structs, "braces", style.spacing.structs.braces);
        SetWhitespacePlacement(structs, "name", style.spacing.structs.name);

        YAML::Node unions = GetMap(spacing, "unions");
        SetPairedWhitespace(unions, "braces", style.spacing.unions.braces);
        SetWhitespacePlacement(unions, "name", style.spacing.unions.name);

        YAML::Node enums = GetMap(spacing, "enums");
        SetPairedWhitespace(enums, "braces", style.spacing.enums.braces);
        SetWhitespacePlacement(enums, "name", style.spacing.enums.name);

        YAML::Node functionDeclarations = GetMap(spacing, "function_declarations");
        SetPairedWhitespace(functionDeclarations, "parentheses", style.spacing.functionDeclarations.parentheses);
        SetWhitespacePlacement(functionDeclarations, "commas", style.spacing.functionDeclarations.commas);
        SetRequiredWhitespace(functionDeclarations, "return_type", style.spacing.functionDeclarations.returnType);

        YAML::Node functionDefinitions = GetMap(spacing, "function_definitions");
        SetPairedWhitespace(functionDefinitions, "parentheses", style.spacing.functionDefinitions.parentheses);
        SetWhitespacePlacement(functionDefinitions, "commas", style.spacing.functionDefinitions.commas);
        SetPairedWhitespace(functionDefinitions, "braces", style.spacing.functionDefinitions.braces);
        SetRequiredWhitespace(functionDefinitions, "return_type", style.spacing.functionDefinitions.returnType);

        YAML::Node functionCalls = GetMap(spacing, "function_calls");
        SetPairedWhitespace(functionCalls, "parentheses", style.spacing.functionCalls.parentheses);
        SetWhitespacePlacement(functionCalls, "commas", style.spacing.functionCalls.commas);

        YAML::Node forLoops = GetMap(spacing, "for_loops");
        SetPairedWhitespace(forLoops, "parentheses", style.spacing.forLoops.parentheses);
        SetWhitespacePlacement(forLoops, "commas", style.spacing.forLoops.commas);
        SetWhitespacePlacement(forLoops, "semicolons", style.spacing.forLoops.semicolons);
        SetWhitespacePlacement(forLoops, "for_each_colon", style.spacing.forLoops.forEachColon);
        SetPairedWhitespace(forLoops, "braces", style.spacing.forLoops.braces);

        YAML::Node whileLoops = GetMap(spacing, "while_loops");
        SetPairedWhitespace(whileLoops, "parentheses", style.spacing.whileLoops.parentheses);
        SetWhitespacePlacement(whileLoops, "commas", style.spacing.whileLoops.commas);
        SetWhitespacePlacement(whileLoops, "semicolons", style.spacing.whileLoops.semicolons);
        SetPairedWhitespace(whileLoops, "braces", style.spacing.whileLoops.braces);

        YAML::Node doWhileLoops = GetMap(spacing, "do_while_loops");
        SetPairedWhitespace(doWhileLoops, "parentheses", style.spacing.doWhileLoops.parentheses);
        SetWhitespacePlacement(doWhileLoops, "commas", style.spacing.doWhileLoops.commas);
        SetWhitespacePlacement(doWhileLoops, "semicolons", style.spacing.doWhileLoops.semicolons);
        SetPairedWhitespace(doWhileLoops, "braces", style.spacing.doWhileLoops.braces);

        YAML::Node ifStatements = GetMap(spacing, "if_statements");
        SetPairedWhitespace(ifStatements, "parentheses", style.spacing.ifStatements.parentheses);
        SetWhitespacePlacement(ifStatements, "commas", style.spacing.ifStatements.commas);
        SetWhitespacePlacement(ifStatements, "semicolons", style.spacing.ifStatements.semicolons);
        SetPairedWhitespace(ifStatements, "braces", style.spacing.ifStatements.braces);
        SetPairedWhitespace(ifStatements, "else_braces", style.spacing.ifStatements.elseBraces);

        YAML::Node switchStatements = GetMap(spacing, "switch_statements");
        SetPairedWhitespace(switchStatements, "parentheses", style.spacing.switchStatements.parentheses);
        SetPairedWhitespace(switchStatements, "braces", style.spacing.switchStatements.braces);

        YAML::Node caseStatements = GetMap(spacing, "switch_statements");
        SetPairedWhitespace(caseStatements, "braces", style.spacing.caseStatements.braces);
        SetWhitespacePlacement(caseStatements, "colon", style.spacing.caseStatements.colon);

        YAML::Node bitFields = GetMap(spacing, "bit_fields");
        SetWhitespacePlacement(bitFields, "colon", style.spacing.bitFields.colon);

        YAML::Node tryCatch = GetMap(spacing, "try_catch");
        SetPairedWhitespace(tryCatch, "braces", style.spacing.tryCatch.braces);
        SetPairedWhitespace(tryCatch, "catch_braces", style.spacing.tryCatch.catchBraces);

        SetPairedWhitespace(spacing, "parentheses", style.spacing.parentheses);
        SetWhitespacePlacement(spacing, "binary_operator", style.spacing.binaryOperator);
        SetWhitespacePlacement(spacing, "pointers", style.spacing.pointers);
        SetWhitespacePlacement(spacing, "references", style.spacing.references);

        SetNewLineType(spacing, "new_line_type", style.spacing.newLineType);
        SetBool(spacing, "respace", style.spacing.respace);
        SetBool(spacing, "trim_trailing", style.spacing.trimTrailing);

        //
        // Alignment
        //
        YAML::Node alignment = GetMap(root, "alignment");

        SetAlignment(alignment, "variable_declarations", style.alignment.variableDeclarations);
        SetAlignment(alignment, "member_variable_declarations", style.alignment.memberVariableDeclarations);
        SetAlignment(alignment, "assignments", style.alignment.assignments);
        SetAlignment(alignment, "bit_fields", style.alignment.bitFields);
        SetAlignment(alignment, "macros", style.alignment.macros);
        SetAlignment(alignment, "escaped_new_lines", style.alignment.escapedNewlines);

        return style;
    }
}
