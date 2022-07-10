#pragma once

#include <tree_sitter/api.h>

extern "C" {
const TSLanguage* tree_sitter_cpp(void);
}

namespace tree_sitter_format {
    
inline TSSymbol IF_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "if_statement", 12, true);;
inline TSSymbol FOR_LOOP = ts_language_symbol_for_name(tree_sitter_cpp(), "for_statement", 13, true);
inline TSSymbol FOR_RANGE_LOOP = ts_language_symbol_for_name(tree_sitter_cpp(), "for_range_loop", 14, true);
inline TSSymbol WHILE_LOOP = ts_language_symbol_for_name(tree_sitter_cpp(), "while_statement", 15, true);
inline TSSymbol DO_WHILE_LOOP = ts_language_symbol_for_name(tree_sitter_cpp(), "do_statement", 12, true);
inline TSSymbol FUNCTION_DEFINITION = ts_language_symbol_for_name(tree_sitter_cpp(), "function_definition", 19, true);
inline TSSymbol SWITCH_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "switch_statement", 16, true);
inline TSSymbol CASE_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "case_statement", 14, true);

// Regular code block
inline TSSymbol COMPOUND_STATEMENT = ts_language_symbol_for_name(tree_sitter_cpp(), "compound_statement", 18, true);
// The block for namespaces and externs
inline TSSymbol DECLARATION_LIST = ts_language_symbol_for_name(tree_sitter_cpp(), "declaration_list", 16, true);
// The block for enums
inline TSSymbol ENUMERATOR_LIST = ts_language_symbol_for_name(tree_sitter_cpp(), "enumerator_list", 15, true);
// The block for structs or classes
inline TSSymbol FIELD_DECLARATION_LIST = ts_language_symbol_for_name(tree_sitter_cpp(), "field_declaration_list", 22, true);
// The block for braced initialiation (designated initialization, struct initialization, etc)
inline TSSymbol INITIALIZER_LIST = ts_language_symbol_for_name(tree_sitter_cpp(), "initializer_list", 16, true);

inline TSSymbol ERROR = ts_language_symbol_for_name(tree_sitter_cpp(), "ERROR", 5, true);

}
