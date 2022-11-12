#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <tree_sitter/api.h>

#include <tree-sitter-format/document/Edits.h>
#include <tree-sitter-format/document/Position.h>
#include <tree-sitter-format/document/Range.h>
#include <tree-sitter-format/document/DocumentSlice.h>

// https://en.wikipedia.org/wiki/Piece_table

namespace tree_sitter_format {

using TSParserDeleter = decltype(&ts_parser_delete);
using TSTreeDeleter = decltype(&ts_tree_delete);

class Document : public DocumentSlice {
private:
    std::string original;

    std::unique_ptr<TSParser, TSParserDeleter> parser;
    std::unique_ptr<TSTree, TSTreeDeleter> tree;
    std::vector<Range> unformattableRanges;

    // Returns the index of the element after the split
    size_t splitAtPosition(uint32_t position);

    static const char* Read(void* payload, uint32_t byte_index, TSPoint position, uint32_t *bytes_read);

    void insertBytes(const Position& position, std::string_view bytes);
    void deleteBytes(const Range& range);

public:
    Document(const std::filesystem::path& file);
    Document(const std::string& contents);
    Document(std::string&& contents);

    void applyEdits(std::vector<Edit> edits);

    const std::string& originalContents() const;
    const std::string_view originalContentsAt(const Range& range) const;

    bool overlapsUnformattableRange(const Range& range) const;

    TSNode root() const;

    TSInput inputReader();
};

}
