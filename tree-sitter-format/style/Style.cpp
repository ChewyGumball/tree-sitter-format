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
        }

        // BraceWrapping 
        // BreakAfterJavaFieldAnnotations 
        // BreakBeforeBinaryOperators 
        // BreakBeforeBraces 
        // BreakBeforeConceptDeclarations 
        // BreakBeforeTernaryOperators 
        // BreakConstructorInitializers 
        // BreakInheritanceList 
        // BreakStringLiterals 
        // ColumnLimit 
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
        // IndentCaseLabels 
        // IndentExternBlock 
        // IndentGotoLabels 
        // IndentPPDirectives 
        // IndentRequiresClause 
        // IndentWidth 
        // IndentWrappedFunctionNames 
        // InsertBraces 
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
        // QualifierAlignment 
        // QualifierOrder 
        // RawStringFormats 
        // ReferenceAlignment 
        // ReflowComments 
        // RemoveBracesLLVM 
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
        // SpaceBeforeCpp11BracedList 
        // SpaceBeforeCtorInitializerColon 
        // SpaceBeforeInheritanceColon 
        // SpaceBeforeParens 
        // SpaceBeforeParensOptions 
        // SpaceBeforeRangeBasedForLoopColon 
        // SpaceBeforeSquareBrackets 
        // SpaceInEmptyBlock 
        // SpaceInEmptyParentheses 
        // SpacesBeforeTrailingComments 
        // SpacesInAngles 
        // SpacesInCStyleCastParentheses 
        // SpacesInConditionalStatement 
        // SpacesInContainerLiterals 
        // SpacesInLineCommentPrefix 
        // SpacesInParentheses 
        // SpacesInSquareBrackets 
        // Standard 
        // StatementAttributeLikeMacros 
        // StatementMacros 

        // TabWidth 
        style.indentation.tabWidth = GetOptionalInt(root, "TabWidth", style.indentation.tabWidth);
        
        // TypenameMacros 
        // UseCRLF 
        // UseTab 
        // WhitespaceSensitiveMacros 

        return style;
    }

    Style Style::FromTreeSitterFormat(const std::string& config) {
        Style style;
        
        YAML::Node root = YAML::Load(config);

        YAML::Node indentation = GetMap(root, "indentation");
        style.indentation.namespaces          = GetOptionalIndentation(indentation, "namespaces", style.indentation.namespaces);
        style.indentation.functionDefinitions = GetOptionalIndentation(indentation, "function_definitions", style.indentation.functionDefinitions);
        style.indentation.structDefinitions   = GetOptionalIndentation(indentation, "struct_definitions", style.indentation.structDefinitions);
        style.indentation.classDefinitions    = GetOptionalIndentation(indentation, "class_definitions", style.indentation.classDefinitions);
        style.indentation.ifStatements        = GetOptionalIndentation(indentation, "if_statements", style.indentation.ifStatements);
        style.indentation.switchStatements    = GetOptionalIndentation(indentation, "switch_statements", style.indentation.switchStatements);
        style.indentation.caseBlocks          = GetOptionalIndentation(indentation, "case_blocks", style.indentation.caseBlocks);
        style.indentation.whileLoops          = GetOptionalIndentation(indentation, "while_loops", style.indentation.whileLoops);
        style.indentation.doWhileLoops        = GetOptionalIndentation(indentation, "do_while_loops", style.indentation.doWhileLoops);
        style.indentation.forLoops            = GetOptionalIndentation(indentation, "for_loops", style.indentation.forLoops);
        style.indentation.genericScope        = GetOptionalIndentation(indentation, "generic_scope", style.indentation.genericScope);

        style.indentation.whitespace        = GetOptionalIndentationWhitespace(indentation, "whitespace", style.indentation.whitespace);
        style.indentation.indentationAmount = GetOptionalInt(indentation, "indentation_amount", style.indentation.indentationAmount);
        style.indentation.tabWidth          = GetOptionalInt(indentation, "tab_width", style.indentation.tabWidth);
        style.indentation.reindent          = GetOptionalBoolean(indentation, "reindent", style.indentation.reindent);


        YAML::Node braces = GetMap(root, "braces");
        style.braces.ifStatements     = GetOptionalBraceExistance(braces, "if_statements", style.braces.ifStatements);
        style.braces.forLoops         = GetOptionalBraceExistance(braces, "for_loops", style.braces.forLoops);
        style.braces.whileLoops       = GetOptionalBraceExistance(braces, "while_loops", style.braces.whileLoops);
        style.braces.doWhileLoops     = GetOptionalBraceExistance(braces, "do_while_loops", style.braces.doWhileLoops);
        style.braces.caseStatements   = GetOptionalBraceExistance(braces, "case_statements", style.braces.caseStatements);
        style.braces.switchStatements = GetOptionalBraceExistance(braces, "switch_statements", style.braces.switchStatements);


        YAML::Node spacing = GetMap(root, "spacing");

        style.spacing.newLineType  = GetOptionalNewLineType(spacing, "new_line_type", style.spacing.newLineType);
        style.spacing.respace      = GetOptionalBoolean(spacing, "respace", style.spacing.respace);
        style.spacing.trimTrailing = GetOptionalBoolean(spacing, "trim_trailing", style.spacing.trimTrailing);

        return style;
    }
}
