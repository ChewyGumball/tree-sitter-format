#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <tree_sitter/api.h>

// https://en.wikipedia.org/wiki/Piece_table

namespace tree_sitter_format {

using TSParserDeleter = decltype(&ts_parser_delete);
using TSTreeDeleter = decltype(&ts_tree_delete);

class Document {
    std::string original;
    std::vector<std::string_view> elements;
    size_t length;

    std::unique_ptr<TSParser, TSParserDeleter> parser;
    std::unique_ptr<TSTree, TSTreeDeleter> tree;

    struct BytePosition {
        size_t index;
        size_t offset;
    };

    BytePosition findBytePosition(uint32_t position) const;

    // Returns the index of the element after the split
    size_t splitAtPosition(uint32_t position);

    friend std::ostream& operator<<(std::ostream& out, const Document& document);

    static const char* Read(void* payload, uint32_t byte_index, TSPoint position, uint32_t *bytes_read);

public:
    struct Range {
        uint32_t start;
        uint32_t end;

        Range(uint32_t start, uint32_t end) : start(start), end(end) {}
        Range(TSNode node) : start(ts_node_start_byte(node)), end(ts_node_end_byte(node)) {}

        uint32_t count() { return end - start; }
    };

    Document(const std::filesystem::path& file);
    Document(const std::string&& contents);

    void insertBytes(uint32_t position, std::string_view bytes);
    void deleteBytes(Range range);

    const std::string& originalContents() const;
    const std::string_view originalContentsAt(Range range) const;

    TSInput inputReader();
};

std::ostream& operator<<(std::ostream& out, const Document& document);

}
