#include <tree-sitter-format/document/Range.h>

namespace tree_sitter_format {
    
Range Range::Of(TSNode node) {
    return Range {
        .start = Position::StartOf(node),
        .end = Position::EndOf(node),
    };
}

Range Range::Between(TSNode start, TSNode end) {
    return Range {
        .start = Position::EndOf(start),
        .end = Position::StartOf(end),
    };
}

Range Range::Between(Position start, Position end) {
    return Range {
        .start = start,
        .end = end,
    };
}

}
