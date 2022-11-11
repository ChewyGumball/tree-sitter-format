#include <tree-sitter-format/Formatter.h>

namespace tree_sitter_format {

    void Formatter::addTraverser(std::unique_ptr<Traverser> traverser) {
        traversers.push_back(std::move(traverser));
    }

    void Formatter::format(const Style& style, Document& document) {
        for (std::unique_ptr<Traverser>& traverser : traversers) {
            std::vector<Edit> edits = traverser->traverse(document, style);
            document.applyEdits(edits);
        }
    }

}
