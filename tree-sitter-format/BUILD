load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_binary", "tsf_cc_library")

tsf_cc_binary(
    name = "tree-sitter-format",
    srcs = ["main.cpp"],
    deps = [
        ":bracket_existance_traverser",
        ":indentation_traverser",
        ":parse_traverser",
        ":space_traverser",
        ":declaration_alignment_traverser",
        ":bitfield_alignment_traverser",
        ":constants",
        ":configuration",
        ":document",
        ":style",
        ":formatter",
        "@tree-sitter-cpp",
        "@yaml-cpp",
    ],
)

tsf_cc_library(
    name = "traverser",
    hdrs = ["traversers/Traverser.h"],
    srcs = ["traversers/Traverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":edits", ":document", ":style", "@tree-sitter"],
)

tsf_cc_library(
    name = "indentation_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/IndentationTraverser.h"],
    srcs = ["traversers/IndentationTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "bracket_existance_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/BracketExistanceTraverser.h"],
    srcs = ["traversers/BracketExistanceTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "space_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/SpaceTraverser.h"],
    srcs = ["traversers/SpaceTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "declaration_alignment_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/DeclarationAlignmentTraverser.h"],
    srcs = ["traversers/DeclarationAlignmentTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "bitfield_alignment_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/BitfieldAlignmentTraverser.h"],
    srcs = ["traversers/BitfieldAlignmentTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "parse_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/ParseTraverser.h"],
    srcs = ["traversers/ParseTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "constants",
    hdrs = ["Constants.h"],
    include_prefix = "tree_sitter_format",
    deps = ["@tree-sitter"],
)

tsf_cc_library(
    name = "util",
    hdrs = ["Util.h"],
    srcs = ["Util.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        "@tree-sitter",
    ],
)

tsf_cc_library(
    name = "style",
    visibility = ["//visibility:public"],
    hdrs = ["style/Style.h"],
    srcs = ["style/Style.cpp"],
    include_prefix = "tree_sitter_format",
    deps = ["@yaml-cpp"],
)

tsf_cc_library(
    name = "configuration",
    hdrs = ["configuration/Configuration.h"],
    srcs = ["configuration/Configuration.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":style"],
)

tsf_cc_library(
    name = "position",
    hdrs = ["document/Position.h"],
    srcs = ["document/Position.cpp"],
    include_prefix = "tree_sitter_format",
    deps = ["@tree-sitter"],
)

tsf_cc_library(
    name = "range",
    hdrs = ["document/Range.h"],
    srcs = ["document/Range.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":position", "@tree-sitter"],
)

tsf_cc_library(
    name = "edits",
    hdrs = ["document/Edits.h"],
    srcs = ["document/Edits.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":position", ":range", "@tree-sitter"],
)

tsf_cc_library(
    name = "document",
    visibility = ["//visibility:public"],
    hdrs = ["document/Document.h"],
    srcs = ["document/Document.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":edits",
        ":position",
        ":range",
        "@tree-sitter",
        "@tree-sitter-cpp",
    ],
)

tsf_cc_library(
    name = "formatter",
    visibility = ["//visibility:public"],
    hdrs = ["Formatter.h"],
    srcs = ["Formatter.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":document", ":style", ":traverser"],
)