#include <tree_sitter_format/style/Style.h>

namespace {
    using namespace tree_sitter_format;
    std::string CreateIndentationString(const Style& style) {
        char c = style.indentation.whitespace == Style::Whitespace::Tabs ? '\t' : ' ';
        return std::string(style.indentation.indentationAmount, c);
    }
}

namespace tree_sitter_format {
    const std::string& Style::indentationString() const {
        static std::string s = CreateIndentationString(*this); 
        return s;
    }
}
