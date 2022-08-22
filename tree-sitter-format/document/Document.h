#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <tree_sitter/api.h>

#include <tree_sitter_format/document/Edits.h>
#include <tree_sitter_format/document/Position.h>
#include <tree_sitter_format/document/Range.h>

// https://en.wikipedia.org/wiki/Piece_table

namespace tree_sitter_format {

using TSParserDeleter = decltype(&ts_parser_delete);
using TSTreeDeleter = decltype(&ts_tree_delete);

class Document {
private:
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

    void insertBytes(Position position, std::string_view bytes);
    void deleteBytes(Range range);

public:
    Document(const std::filesystem::path& file);
    Document(const std::string& contents);
    Document(std::string&& contents);

    void applyEdits(std::vector<Edit> edits);

    const std::string& originalContents() const;
    const std::string_view originalContentsAt(Range range) const;
    std::string toString() const;

    std::string contentsAt(Range range) const;
    char characterAt(uint32_t bytePosition) const;

    Range toNextNewLine(Position start) const;
    Range toPreviousNewLine(Position end) const;

    TSNode root() const;

    TSInput inputReader();
};

std::ostream& operator<<(std::ostream& out, const Document& document);

}
