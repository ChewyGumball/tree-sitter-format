#pragma once

#include <memory>

#include <tree_sitter_format/traversers/Traverser.h>

namespace tree_sitter_format {

class Formatter {
private:
    std::vector<std::unique_ptr<Traverser>> traversers;

public:
    void addTraverser(std::unique_ptr<Traverser> traverser);
    void format(const Style& style, Document& document);
};

}
