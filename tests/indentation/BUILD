load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_test")

tsf_cc_test(
    name = "namespaces",
    srcs = ["Namespaces.cpp"],
    deps = [
        "//tree-sitter-format:formatter",
        "//tree-sitter-format:indentation_traverser",
    ]
)

tsf_cc_test(
    name = "for_loops",
    srcs = ["ForLoops.cpp"],
    deps = [
        "//tree-sitter-format:formatter",
        "//tree-sitter-format:indentation_traverser",
    ]
)

tsf_cc_test(
    name = "for_each_loops",
    srcs = ["ForEachLoops.cpp"],
    deps = [
        "//tree-sitter-format:formatter",
        "//tree-sitter-format:indentation_traverser",
    ]
)