#pragma once

#include <cstdint>
#include <string>

namespace tree_sitter_format {

struct Style {
    uint32_t targetLineLength = 120;

    struct {
        bool reflow = true;
        bool alignTrailing = false;
    } comments;

    enum class IndentationWhitespace { Spaces, Tabs };
    enum class Indentation { BracesIndented, BodyIndented, BothIndented, None };

    struct {
        Indentation namespaces = Indentation::None;
        Indentation functionDefinitions = Indentation::BodyIndented;
        Indentation structDefinitions = Indentation::BodyIndented;
        Indentation classDefinitions = Indentation::BodyIndented;
        Indentation ifStatements = Indentation::BodyIndented;
        Indentation switchStatements = Indentation::BodyIndented;
        Indentation caseBlocks = Indentation::BodyIndented;
        Indentation whileLoops = Indentation::BodyIndented;
        Indentation doWhileLoops = Indentation::BodyIndented;
        Indentation forLoops = Indentation::BodyIndented;
        Indentation genericScope = Indentation::BodyIndented;
        Indentation tryCatch = Indentation::BodyIndented;
        Indentation lambdas = Indentation::BodyIndented;

        IndentationWhitespace whitespace = IndentationWhitespace::Spaces;
        uint32_t indentationAmount = 4;
        uint32_t tabWidth = 4;
        bool reindent = true;
    } indentation;

    std::string_view indentationString() const;

    enum class BraceExistance { Require, Remove, Ignore };

    struct {
        BraceExistance ifStatements = BraceExistance::Require;
        BraceExistance forLoops = BraceExistance::Require;
        BraceExistance whileLoops = BraceExistance::Require;
        BraceExistance doWhileLoops = BraceExistance::Require;
        BraceExistance caseStatements = BraceExistance::Require;
        BraceExistance switchStatements = BraceExistance::Require;
    } braces;

    enum class Whitespace { Space, Newline, Ignore, None };
    enum class RequiredWhitespace { Space, Newline, Ignore };
    struct WhitespacePlacement {
        Whitespace before;
        Whitespace after;
    };

    struct PairedWhitespace {
        WhitespacePlacement opening;
        WhitespacePlacement closing;
    };

    enum class NewLineType { CRLF, LF, CR };
    struct {
        struct {
            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };
        } namespaces;

        struct {
            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };

            WhitespacePlacement name {
                .before = Whitespace::Space,
                .after = Whitespace::None,
            };
        } classes;
        
        struct {
            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };

            WhitespacePlacement name {
                .before = Whitespace::Space,
                .after = Whitespace::None,
            };
        } structs;
        
        struct {
            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };

            WhitespacePlacement name {
                .before = Whitespace::Space,
                .after = Whitespace::None,
            };
        } unions;

        struct {
            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };

            WhitespacePlacement name {
                .before = Whitespace::Space,
                .after = Whitespace::None,
            };
        } enums;

        struct {
            PairedWhitespace parentheses {
                .opening = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
            };

            WhitespacePlacement commas {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            RequiredWhitespace returnType;
        } functionDeclarations;

        struct {
            PairedWhitespace parentheses {
                .opening = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
            };

            WhitespacePlacement commas {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };
            
            RequiredWhitespace returnType;
            WhitespacePlacement trailingReturnTypeArrow;
        } functionDefinitions;

        struct {
            PairedWhitespace parentheses {
                .opening = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
            };

            WhitespacePlacement commas {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };
        } functionCalls;

        struct {
            PairedWhitespace parentheses {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::None,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
            };

            WhitespacePlacement commas {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            WhitespacePlacement semicolons {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            WhitespacePlacement forEachColon {
                .before = Whitespace::Space,
                .after = Whitespace::Space,
            };

            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };
        } forLoops;

        struct {            
            PairedWhitespace parentheses {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::None,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
            };

            WhitespacePlacement commas {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            WhitespacePlacement semicolons {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };
        } whileLoops;

        struct {            
            PairedWhitespace parentheses {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::None,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
            };

            WhitespacePlacement commas {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            WhitespacePlacement semicolons {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Space,
                },
            };
        } doWhileLoops;

        struct {            
            PairedWhitespace parentheses {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::None,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
            };

            WhitespacePlacement commas {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            WhitespacePlacement semicolons {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };

            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };

            PairedWhitespace elseBraces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Space,
                },
            };
        } ifStatements;

        struct {
            PairedWhitespace parentheses {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::None,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::None,
                    .after = Whitespace::None,
                },
            };

            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };
        } switchStatements;

        struct {
            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Newline,
                },
            };

            WhitespacePlacement colon {
                .before = Whitespace::None,
                .after = Whitespace::None,
            };
        } caseStatements;

        struct {
            WhitespacePlacement colon {
                .before = Whitespace::None,
                .after = Whitespace::Space,
            };
        } bitFields;

        struct {
            PairedWhitespace braces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Space,
                },
            };
            PairedWhitespace catchBraces {
                .opening = WhitespacePlacement {
                    .before = Whitespace::Space,
                    .after = Whitespace::Newline,
                },
                .closing = WhitespacePlacement {
                    .before = Whitespace::Newline,
                    .after = Whitespace::Space,
                },
            };
        } tryCatch;

        struct {
            struct {
                PairedWhitespace squareBrackets {
                    .opening = WhitespacePlacement {
                        .before = Whitespace::None,
                        .after = Whitespace::None,
                    },
                    .closing = WhitespacePlacement {
                        .before = Whitespace::None,
                        .after = Whitespace::None,
                    },
                };

                WhitespacePlacement commas {
                    .before = Whitespace::None,
                    .after = Whitespace::Space,
                };
            } captureList;

            struct {
                PairedWhitespace parentheses {
                    .opening = WhitespacePlacement {
                        .before = Whitespace::None,
                        .after = Whitespace::None,
                    },
                    .closing = WhitespacePlacement {
                        .before = Whitespace::None,
                        .after = Whitespace::None,
                    },
                };

                WhitespacePlacement commas {
                    .before = Whitespace::None,
                    .after = Whitespace::Space,
                };

                WhitespacePlacement trailingReturnTypeArrow;
            } signature;

            struct {
                PairedWhitespace braces {
                    .opening = WhitespacePlacement {
                        .before = Whitespace::Space,
                        .after = Whitespace::Newline,
                    },
                    .closing = WhitespacePlacement {
                        .before = Whitespace::Newline,
                        .after = Whitespace::Newline,
                    },
                };
             } body;
        } lambdas;
    
        PairedWhitespace parentheses {
            .opening = WhitespacePlacement {
                .before = Whitespace::Space,
                .after = Whitespace::None,
            },
            .closing = WhitespacePlacement {
                .before = Whitespace::None,
                .after = Whitespace::None,
            },
        };

        WhitespacePlacement binaryOperator {
            .before = Whitespace::Space,
            .after = Whitespace::Space,
        };

        WhitespacePlacement pointers {
            .before = Whitespace::None,
            .after = Whitespace::Space,
        };

        WhitespacePlacement references {
            .before = Whitespace::None,
            .after = Whitespace::Space,
        };

        NewLineType newLineType = NewLineType::LF;
        bool respace = true;
        bool trimTrailing = true;
    } spacing;
    
    struct Alignment {
        bool align;
        bool acrossComments;
        bool acrossEmptyLines;
    };

    enum class Justify { Left, Right };

    struct {
        struct {
            Alignment alignment  {
                .align = true,
                .acrossComments = false,
                .acrossEmptyLines = false,
            };

            bool alignCommasSeparately = false;

            Justify justification = Justify::Left;
        }  initializerLists;
        
        Alignment variableDeclarations {
            .align = true,
            .acrossComments = false,
            .acrossEmptyLines = false,
        };
        Alignment memberVariableDeclarations {
            .align = true,
            .acrossComments = false,
            .acrossEmptyLines = false,
        };
        Alignment assignments {
            .align = true,
            .acrossComments = false,
            .acrossEmptyLines = false,
        };
        Alignment bitFields {
            .align = true,
            .acrossComments = false,
            .acrossEmptyLines = false,
        };
        Alignment macros {
            .align = true,
            .acrossComments = false,
            .acrossEmptyLines = false,
        };
        Alignment escapedNewlines {
            .align = true,
            .acrossComments = true,
            .acrossEmptyLines = false,
        };
    } alignment;

    std::string_view newLineString() const;

    static Style FromClangFormat(const std::string& config);
    static Style FromTreeSitterFormat(const std::string& config);
};

}
