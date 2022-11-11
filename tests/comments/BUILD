load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_test")

tsf_cc_test(
    name = "multiline",
    srcs = ["Multiline.cpp"],
    deps = [
        "//tree-sitter-format/traversers:multiline_comment_reflow_traverser",
        "//tests:test_utils",
    ]
)