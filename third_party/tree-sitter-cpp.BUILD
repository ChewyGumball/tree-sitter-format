load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_library")

WINDOWS_WARNING_SUPPRESSIONS = [
    "-wd4100", # 'valid_symbols': unreferenced formal parameter
    "-wd4244", # 'argument': conversion from 'int32_t' to 'wint_t', possible loss of data
]

tsf_cc_library(
    name = "tree-sitter-cpp",
    srcs = [
        "src/parser.c",
        "src/scanner.cc",
    ],
    copts = select({
        "@platforms//os:windows": WINDOWS_WARNING_SUPPRESSIONS,
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = ["@tree-sitter"],
)