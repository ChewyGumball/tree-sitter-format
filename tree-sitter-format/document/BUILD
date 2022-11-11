load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_library")

tsf_cc_library(
    name = "position",
    hdrs = ["Position.h"],
    srcs = ["Position.cpp"],
    deps = ["@tree-sitter"],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "range",
    hdrs = ["Range.h"],
    srcs = ["Range.cpp"],
    deps = [
        ":position",
        "@tree-sitter"
    ],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "unicode_iterator",
    hdrs = ["UnicodeIterator.h"],
    srcs = ["UnicodeIterator.cpp"],
    deps = [
        ":position",
        ":range",
    ],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "edits",
    hdrs = ["Edits.h"],
    srcs = ["Edits.cpp"],
    deps = [
        ":position",
        ":range",
        "@tree-sitter"
    ],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "document_slice",
    hdrs = ["DocumentSlice.h"],
    srcs = ["DocumentSlice.cpp"],
    deps = [
        ":unicode_iterator",
        ":position",
        ":range",
        "//tree-sitter-format:util",
        "@tree-sitter"
    ],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "document",
    hdrs = ["Document.h"],
    srcs = ["Document.cpp"],
    deps = [
        ":document_slice",
        ":edits",
        ":position",
        ":range",
        "@tree-sitter",
        "@tree-sitter-cpp",
    ],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "text_reflower",
    hdrs = ["TextReflower.h"],
    srcs = ["TextReflower.cpp"],
    deps = [
        ":document_slice",
        "//tree-sitter-format:util"
    ],

    visibility = ["//visibility:public"],
)