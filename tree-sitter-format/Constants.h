#pragma once

#include <tree_sitter/api.h>

extern "C" {
const TSLanguage* tree_sitter_cpp(void);
}

namespace tree_sitter_format {
    
inline const TSSymbol IF_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "if_statement", 12, true);;
inline const TSSymbol FOR_LOOP = ts_language_symbol_for_name(tree_sitter_cpp(), "for_statement", 13, true);
inline const TSSymbol FOR_RANGE_LOOP = ts_language_symbol_for_name(tree_sitter_cpp(), "for_range_loop", 14, true);
inline const TSSymbol WHILE_LOOP = ts_language_symbol_for_name(tree_sitter_cpp(), "while_statement", 15, true);
inline const TSSymbol DO_WHILE_LOOP = ts_language_symbol_for_name(tree_sitter_cpp(), "do_statement", 12, true);
inline const TSSymbol FUNCTION_DEFINITION = ts_language_symbol_for_name(tree_sitter_cpp(), "function_definition", 19, true);
inline const TSSymbol SWITCH_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "switch_statement", 16, true);
inline const TSSymbol CASE_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "case_statement", 14, true);
inline const TSSymbol EXPRESSION_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "expression_statement", 20, true);
inline const TSSymbol ASSIGNMENT_EXPRESSION = ts_language_symbol_for_name(tree_sitter_cpp(), "assignment_expression", 21, true);
inline const TSSymbol NAMESPACE = ts_language_symbol_for_name(tree_sitter_cpp(), "namespace_definition", 20, true);
inline const TSSymbol STRUCT_DEFINITION = ts_language_symbol_for_name(tree_sitter_cpp(), "struct_specifier", 16, true);
inline const TSSymbol CLASS_DEFINITION = ts_language_symbol_for_name(tree_sitter_cpp(), "class_specifier", 15, true);
inline const TSSymbol LAMBDA_EXPRESSION = ts_language_symbol_for_name(tree_sitter_cpp(), "lambda_expression", 17, true);
inline const TSSymbol TRY_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "try_statement", 13, true);
inline const TSSymbol CATCH_CLAUSE = ts_language_symbol_for_name(tree_sitter_cpp(), "catch_clause", 12, true);

inline const TSSymbol DECLARATION = ts_language_symbol_for_name(tree_sitter_cpp(), "declaration", 11, true);
inline const TSSymbol INIT_DECLARATOR = ts_language_symbol_for_name(tree_sitter_cpp(), "init_declarator", 15, true);
inline const TSSymbol IDENTIFIER = ts_language_symbol_for_name(tree_sitter_cpp(), "identifier", 10, true);
inline const TSSymbol POINTER_DECLARATOR = ts_language_symbol_for_name(tree_sitter_cpp(), "pointer_declarator", 18, true);
inline const TSSymbol ARRAY_DECLARATOR = ts_language_symbol_for_name(tree_sitter_cpp(), "array_declarator", 16, true);
inline const TSSymbol ATTRIBUTED_DECLARATOR = ts_language_symbol_for_name(tree_sitter_cpp(), "attributed_declarator", 21, true);
inline const TSSymbol FUNCTION_DECLARATOR = ts_language_symbol_for_name(tree_sitter_cpp(), "function_declarator", 19, true);
inline const TSSymbol PARENTHESIZED_DECLARATOR = ts_language_symbol_for_name(tree_sitter_cpp(), "parenthesized_declarator", 24, true);
inline const TSSymbol FIELD_DECLARATOR = ts_language_symbol_for_name(tree_sitter_cpp(), "field_declarator", 16, true);

inline const TSSymbol FIELD_DECLARATION = ts_language_symbol_for_name(tree_sitter_cpp(), "field_declaration", 17, true);
inline const TSSymbol FIELD_IDENTIFIER = ts_language_symbol_for_name(tree_sitter_cpp(), "field_identifier", 16, true);

inline const TSSymbol BITFIELD_CLAUSE = ts_language_symbol_for_name(tree_sitter_cpp(), "bitfield_clause", 15, true);

// Regular code block
inline const TSSymbol COMPOUND_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "compound_statement", 18, true);
// The block for namespaces and externs
inline const TSSymbol DECLARATION_LIST = ts_language_symbol_for_name(tree_sitter_cpp(), "declaration_list", 16, true);
// The block for enums
inline const TSSymbol ENUMERATOR_LIST = ts_language_symbol_for_name(tree_sitter_cpp(), "enumerator_list", 15, true);
// The block for structs or classes
inline const TSSymbol FIELD_DECLARATION_LIST = ts_language_symbol_for_name(tree_sitter_cpp(), "field_declaration_list", 22, true);
// The block for braced initialiation (designated initialization, struct initialization, etc)
inline const TSSymbol INITIALIZER_LIST = ts_language_symbol_for_name(tree_sitter_cpp(), "initializer_list", 16, true);

inline const TSSymbol EXPRESSION = ts_language_symbol_for_name(tree_sitter_cpp(), "expression", 10, true);
inline const TSSymbol COMMA_EXPRESSION = ts_language_symbol_for_name(tree_sitter_cpp(), "comma_expression", 16, true);
inline const TSSymbol BINARY_EXPRESSION = ts_language_symbol_for_name(tree_sitter_cpp(), "binary_expression", 17, true);

inline const TSSymbol COMMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "comment", 7, true);

inline const TSSymbol TRANSLATION_UNIT = ts_language_symbol_for_name(tree_sitter_cpp(), "translation_unit", 16, true);
inline const TSSymbol ERROR = ts_language_symbol_for_name(tree_sitter_cpp(), "ERROR", 5, true);

}
