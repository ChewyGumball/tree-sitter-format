#include <tree_sitter_format/style/Style.h>

namespace {
    using namespace tree_sitter_format;
    std::string CreateIndentationString(const Style& style) {
        char c = style.indentation.whitespace == Style::IndentationWhitespace::Tabs ? '\t' : ' ';
        return std::string(style.indentation.indentationAmount, c);
    }

    std::string CreateNewLineString(const Style& style) {
        switch (style.spacing.newLineType) {
            case Style::NewLineType::CRLF: return "\r\n";
            case Style::NewLineType::LF: return "\n";
            case Style::NewLineType::CR: return "\r";
            default: return "\n";
        }
    }
}

namespace tree_sitter_format {
    std::string_view Style::indentationString() const {
        static std::string s = CreateIndentationString(*this); 
        return s;
    }
    std::string_view Style::newLineString() const {
        static std::string s = CreateNewLineString(*this); 
        return s;
    }
}
