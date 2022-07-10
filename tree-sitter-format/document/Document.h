#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <tree_sitter/api.h>

// https://en.wikipedia.org/wiki/Piece_table

namespace tree_sitter_format {

struct Position {
    TSPoint location;
    uint32_t byteOffset;

    static Position StartOf(TSNode node) {
        return Position {
            .location = ts_node_start_point(node),
            .byteOffset = ts_node_start_byte(node),
        };
    }
    static Position EndOf(TSNode node) {
        return Position {
            .location = ts_node_end_point(node),
            .byteOffset = ts_node_end_byte(node),
        };
    }
};

struct Range {
    Position start;
    Position end;

    uint32_t byteCount() const { return end.byteOffset - start.byteOffset; }

    static Range Of(TSNode node) {
        return Range {
            .start = Position::StartOf(node),
            .end = Position::EndOf(node),
        };
    }
    static Range Between(TSNode start, TSNode end) {
        return Range {
            .start = Position::EndOf(start),
            .end = Position::StartOf(end),
        };
    }
};

struct DeleteEdit {
    Range range;
};

struct InsertEdit {
    Position position;
    std::string_view bytes;
};

using Edit = std::variant<DeleteEdit, InsertEdit>;


using TSParserDeleter = decltype(&ts_parser_delete);
using TSTreeDeleter = decltype(&ts_tree_delete);

class Document {
    std::string original;
    std::vector<std::string_view> elements;
    size_t length;

    std::unique_ptr<TSParser, TSParserDeleter> parser;
    std::unique_ptr<TSTree, TSTreeDeleter> tree;

    struct Location {
        size_t index;
        size_t offset;
    };

    Location findByteLocation(uint32_t position) const;

    // Returns the index of the element after the split
    size_t splitAtPosition(uint32_t position);

    friend std::ostream& operator<<(std::ostream& out, const Document& document);

    static const char* Read(void* payload, uint32_t byte_index, TSPoint position, uint32_t *bytes_read);

public:
    Document(const std::filesystem::path& file);
    Document(const std::string&& contents);

    void insertBefore(TSNode node, std::string_view bytes);
    void insertAfter(TSNode node, std::string_view bytes);

    void insertBytes(Position position, std::string_view bytes);
    void deleteBytes(Range range);

    void applyEdits(const std::vector<Edit>& edits);

    const std::string& originalContents() const;
    const std::string_view originalContentsAt(Range range) const;

    std::string contentsAt(Range range) const;
    char characterAt(uint32_t bytePosition) const;

    void reparse();
    TSNode root() const;

    TSInput inputReader();
};

std::ostream& operator<<(std::ostream& out, const Document& document);

}
