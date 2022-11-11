#include <tree-sitter-format/document/Position.h>

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

bool Position::operator==(const Position& other) const {
    return location.row == other.location.row && location.column == other.location.column;
}

std::strong_ordering Position::operator<=>(const Position& other) const {
    if (location.row == other.location.row) {
        return location.column <=> other.location.column;
    } else {
        return location.row <=> other.location.row;
    }
}

}
