#include <tree_sitter_format/document/Edits.h>

namespace {
    struct EditStartVisit {
        uint32_t operator()(const tree_sitter_format::DeleteEdit& d) { return d.range.start.byteOffset; }
        uint32_t operator()(const tree_sitter_format::InsertEdit& i) { return i.position.byteOffset; }
    };
}

namespace tree_sitter_format {

bool operator<(const Edit& lhs, const Edit& rhs) {
        bool aIsDelete = std::holds_alternative<DeleteEdit>(lhs);
        bool bIsDelete = std::holds_alternative<DeleteEdit>(rhs);

        uint32_t aStart = std::visit(EditStartVisit(), lhs);            
        uint32_t bStart = std::visit(EditStartVisit(), rhs);

        if (aStart == bStart && aIsDelete && !bIsDelete) {
            return true;
        } else {
            return aStart > bStart;
        }
}

}
