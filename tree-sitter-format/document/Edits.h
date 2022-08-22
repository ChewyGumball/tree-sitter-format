#pragma once

#include <string_view>
#include <variant>

#include <tree_sitter_format/document/Position.h>
#include <tree_sitter_format/document/Range.h>

namespace tree_sitter_format {

struct DeleteEdit {
    Range range;
};

struct InsertEdit {
    Position position;
    std::string_view bytes;
};

using Edit = std::variant<DeleteEdit, InsertEdit>;

bool operator<(const Edit& lhs, const Edit& rhs);

}
