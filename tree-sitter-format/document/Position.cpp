#include <tree_sitter_format/document/Position.h>

namespace tree_sitter_format {

Position Position::StartOf(TSNode node) {
    return Position {
        .location = ts_node_start_point(node),
        .byteOffset = ts_node_start_byte(node),
    };
}

Position Position::EndOf(TSNode node) {
    return Position {
        .location = ts_node_end_point(node),
        .byteOffset = ts_node_end_byte(node),
    };
}


std::strong_ordering operator<=>(const Position& lhs, const Position& rhs) {
    if (lhs.location.row == rhs.location.row) {
        return lhs.location.column <=> rhs.location.column;
    } else {
        return lhs.location.row <=> rhs.location.row;
    }
}

}
