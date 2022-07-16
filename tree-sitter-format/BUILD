load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_binary", "tsf_cc_library")

tsf_cc_binary(
    name = "tree-sitter-format",
    srcs = ["main.cpp"],
    deps = [
        ":bracket_existance_traverser",
        ":indentation_traverser",
        ":space_traverser",
        ":constants",
        ":configuration",
        ":document",
        ":style",
        ":traverser",
        "@tree-sitter-cpp",
        "@yaml-cpp",
    ],
)

tsf_cc_library(
    name = "traverser",
    hdrs = ["traversers/Traverser.h"],
    srcs = ["traversers/Traverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = ["@tree-sitter"],
)

tsf_cc_library(
    name = "indentation_traverser",
    hdrs = ["traversers/IndentationTraverser.h"],
    srcs = ["traversers/IndentationTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":document",
        ":traverser",
        ":style",
    ],
)

tsf_cc_library(
    name = "bracket_existance_traverser",
    hdrs = ["traversers/BracketExistanceTraverser.h"],
    srcs = ["traversers/BracketExistanceTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":document",
        ":traverser",
        ":style",
    ],
)

tsf_cc_library(
    name = "space_traverser",
    hdrs = ["traversers/SpaceTraverser.h"],
    srcs = ["traversers/SpaceTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":document",
        ":traverser",
        ":style",
    ],
)

tsf_cc_library(
    name = "constants",
    hdrs = ["Constants.h"],
    include_prefix = "tree_sitter_format",
)

tsf_cc_library(
    name = "style",
    hdrs = ["style/Style.h"],
    srcs = ["style/Style.cpp"],
    include_prefix = "tree_sitter_format",
)

tsf_cc_library(
    name = "configuration",
    hdrs = ["configuration/Configuration.h"],
    srcs = ["configuration/Configuration.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":style"],
)

tsf_cc_library(
    name = "document", 
    hdrs = ["document/Document.h"],
    srcs = ["document/Document.cpp"],
    include_prefix = "tree_sitter_format",
    deps = ["@tree-sitter"],
)