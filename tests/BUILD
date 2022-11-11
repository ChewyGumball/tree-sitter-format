load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_library")

tsf_cc_library(
    name = "test_utils",
    hdrs = ["TestUtils.h"],
    visibility = ["//visibility:public"],
    testonly = True,
    deps = [
        "//tree-sitter-format/style",
        "//tree-sitter-format:formatter",
        "@catch2",
    ],
)