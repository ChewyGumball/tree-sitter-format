load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_binary", "tsf_cc_library")

tsf_cc_binary(
    name = "tree-sitter-format",
    srcs = ["main.cpp"],
    deps = [
        ":configuration",
        ":document",
        ":style",
        "@tree-sitter-cpp",
        "@yaml-cpp",
    ],
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