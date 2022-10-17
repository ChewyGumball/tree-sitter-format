#pragma once

#include <tree_sitter/api.h>

#include <string_view>

namespace tree_sitter_format {

[[nodiscard]] TSNode NullNode();

[[nodiscard]] std::string_view ChildFieldName(TSNode node, uint32_t childIndex);
[[nodiscard]] TSNode FindFirstNonExtraChild(TSNode node, uint32_t startingIndex);
[[nodiscard]] TSNode FindLastNonExtraChild(TSNode node, uint32_t startingIndex);

[[nodiscard]] bool IsCompoundStatementLike(TSNode node);
[[nodiscard]] bool IsCaseWithSingleStatementBody(TSNode node);
[[nodiscard]] bool IsDeclarationLike(TSNode node);
[[nodiscard]] bool IsIdentifierLike(TSNode node);
[[nodiscard]] bool IsAssignmentLike(TSNode node);


[[nodiscard]] bool IsBitfieldDeclaration(TSNode node);

[[nodiscard]] std::string_view GetSpaces(uint32_t count);

}
