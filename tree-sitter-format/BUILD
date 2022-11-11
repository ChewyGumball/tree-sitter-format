load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_binary", "tsf_cc_library")

tsf_cc_binary(
    name = "tree-sitter-format",
    srcs = ["main.cpp"],
    deps = [
        "//tree-sitter-format/traversers:bracket_existance_traverser",
        "//tree-sitter-format/traversers:indentation_traverser",
        "//tree-sitter-format/traversers:parse_traverser",
        "//tree-sitter-format/traversers:space_traverser",
        "//tree-sitter-format/traversers:declaration_alignment_traverser",
        "//tree-sitter-format/traversers:bitfield_alignment_traverser",
        "//tree-sitter-format/traversers:assignment_alignment_traverser",
        "//tree-sitter-format/traversers:initializer_list_alignment_traverser",
        "//tree-sitter-format/configuration",
        "//tree-sitter-format/document",
        "//tree-sitter-format/style",
        ":constants",
        ":formatter",
        "@tree-sitter-cpp",
        "@yaml-cpp",
    ],
)

tsf_cc_library(
    name = "constants",
    hdrs = ["Constants.h"],
    deps = ["@tree-sitter"],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "util",
    hdrs = ["Util.h"],
    srcs = ["Util.cpp"],
    deps = [
        ":constants",
        "@tree-sitter",
    ],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "formatter",
    hdrs = ["Formatter.h"],
    srcs = ["Formatter.cpp"],
    deps = [
        "//tree-sitter-format/document",
        "//tree-sitter-format/style",
        "//tree-sitter-format/traversers:traverser"
    ],

    visibility = ["//visibility:public"],
)