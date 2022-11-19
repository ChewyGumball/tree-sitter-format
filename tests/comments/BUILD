load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_test")

tsf_cc_test(
    name = "multiline",
    srcs = ["Multiline.cpp"],
    deps = [
        "//tree-sitter-format/traversers:multiline_comment_reflow_traverser",
        "//tests:test_utils",
    ]
)
tsf_cc_test(
    name = "trailing",
    srcs = ["Trailing.cpp"],
    deps = [
        "//tree-sitter-format/traversers:comment_alignment_traverser",
        "//tests:test_utils",
    ]
)