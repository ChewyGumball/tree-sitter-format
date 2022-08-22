#pragma once

#include <tree_sitter_format/document/Position.h>

namespace tree_sitter_format {

struct Range {
    Position start;
    Position end;

    uint32_t byteCount() const { return end.byteOffset - start.byteOffset; }

    static Range Of(TSNode node);
    static Range Between(TSNode start, TSNode end);
    static Range Between(Position start, Position end);
};

}
