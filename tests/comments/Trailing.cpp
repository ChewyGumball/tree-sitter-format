#include <catch2/catch_test_macros.hpp>

#include <tree-sitter-format/Formatter.h>
#include <tree-sitter-format/traversers/CommentAlignmentTraverser.h>

using namespace tree_sitter_format;

std::string Align(const std::string& input, const Style& style) {
    Formatter formatter;
    formatter.addTraverser(std::make_unique<CommentAlignmentTraverser>());

    Document document(input);
    formatter.format(style, document);

    return document.toString();
}

TEST_CASE("Left Justify") {
    std::string input = R"(
              // sodifj
int a; // good
int baosdijfoisjdf;                     // oh no
for (int i = 0; i < 4;     //sef
    i -= 2) {
            // ok
    }

int b; // opij

int casd; // asd
int sdoijfosdifjosdijf;       //osdijf
int sodifjfjsod;  // os
)";

    std::string expected = R"(
              // sodifj
int a; // good
int baosdijfoisjdf; // oh no
for (int i = 0; i < 4; //sef
    i -= 2) {
            // ok
    }

int b; // opij

int casd; // asd
int sdoijfosdifjosdijf; //osdijf
int sodifjfjsod; // os
)";
    Style style;
    style.alignment.trailingComments = Style::TrailingCommentAlignment::LeftJustify;

    std::string actual = Align(input, style);
    REQUIRE(actual == expected);
}

TEST_CASE("Align Consecutive") {
    std::string input = R"(
              // sodifj
int a;                 // good
int baosdijfoisjdf;                     // oh no
for (int i = 0; i < 4;     //sef
    i -= 2) {
            // ok
    }

int b; // opij

int casd; // asd
int sdoijfosdifjosdijf;       //osdijf
int sodifjfjsod;  // os
)";

    std::string expected = R"(
              // sodifj
int a;                 // good
int baosdijfoisjdf;    // oh no
for (int i = 0; i < 4; //sef
    i -= 2) {
            // ok
    }

int b; // opij

int casd;               // asd
int sdoijfosdifjosdijf; //osdijf
int sodifjfjsod;        // os
)";
    Style style;
    style.alignment.trailingComments = Style::TrailingCommentAlignment::AlignConsecutive;

    std::string actual = Align(input, style);
    REQUIRE(actual == expected);
}


TEST_CASE("Ignore") {
    std::string input = R"(
              // sodifj
int a; // good
int baosdijfoisjdf;                     // oh no
for (int i = 0; i < 4;     //sef
    i -= 2) {
            // ok
    }

int b; // opij

int casd; // asd
int sdoijfosdifjosdijf;       //osdijf
int sodifjfjsod;  // os
)";

    std::string expected = R"(
              // sodifj
int a; // good
int baosdijfoisjdf;                     // oh no
for (int i = 0; i < 4;     //sef
    i -= 2) {
            // ok
    }

int b; // opij

int casd; // asd
int sdoijfosdifjosdijf;       //osdijf
int sodifjfjsod;  // os
)";
    Style style;
    style.alignment.trailingComments = Style::TrailingCommentAlignment::Ignore;

    std::string actual = Align(input, style);
    REQUIRE(actual == expected);
}
