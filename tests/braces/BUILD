load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_test")

tsf_cc_test(
    name = "if_statements",
    srcs = ["IfStatements.cpp"],
    deps = [
        "//tree-sitter-format:bracket_existance_traverser",
        "//tests:test_utils",
    ]
)

tsf_cc_test(
    name = "for_loops",
    srcs = ["ForLoops.cpp"],
    deps = [
        "//tree-sitter-format:bracket_existance_traverser",
        "//tests:test_utils",
    ]
)