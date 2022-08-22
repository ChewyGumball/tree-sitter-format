#pragma once

#include <compare>

#include <tree_sitter/api.h>

namespace tree_sitter_format {

struct Position {
    TSPoint location = TSPoint { .row = 0, .column = 0 };
    uint32_t byteOffset = 0;

    static Position StartOf(TSNode node);
    static Position EndOf(TSNode node);
};

std::strong_ordering operator<=>(const Position& lhs, const Position& rhs);

}
