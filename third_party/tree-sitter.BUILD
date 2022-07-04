load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_library")

WINDOWS_WARNING_SUPPRESSIONS = [
    "-wd4018", # '>': signed/unsigned mismatch,
    "-wd4201", # nonstandard extension used: nameless struct/union
    "-wd4232", # nonstandard extension used: 'ts_current_free': address of dllimport 'free' is not static, identity not guaranteed
    "-wd4244", # 'initializing': conversion from 'uint32_t' to 'TSSymbol', possible loss of data
    "-wd4267", # 'function': conversion from 'size_t' to 'uint32_t', possible loss of data
    "-wd4456", # declaration of 'i' hides previous local declaration
    "-wd4457", # declaration of 'edit' hides function parameter
]

tsf_cc_library(
    name = "tree-sitter",
    srcs = [
        "lib/src/unicode/ptypes.h",
        "lib/src/unicode/umachine.h",
        "lib/src/unicode/urename.h",
        "lib/src/unicode/utf.h",
        "lib/src/unicode/utf16.h",
        "lib/src/unicode/utf8.h",
        "lib/src/alloc.c",
        "lib/src/alloc.h",
        "lib/src/array.h",
        "lib/src/atomic.h",
        "lib/src/clock.h",
        "lib/src/error_costs.h",
        "lib/src/get_changed_ranges.c",
        "lib/src/get_changed_ranges.h",
        "lib/src/host.h",
        "lib/src/language.c",
        "lib/src/language.h",
        "lib/src/length.h",
        "lib/src/lexer.c",
        "lib/src/lexer.h",
        # This file just includes all the other .c files after defining the _POSIX_C_SOURCE macro.
        # "lib/src/lib.c",
        "lib/src/node.c",
        "lib/src/parser.c",
        "lib/src/point.h",
        "lib/src/query.c",
        "lib/src/reduce_action.h",
        "lib/src/reusable_node.h",
        "lib/src/stack.c",
        "lib/src/stack.h",
        "lib/src/subtree.c",
        "lib/src/subtree.h",
        "lib/src/tree.c",
        "lib/src/tree.h",
        "lib/src/tree_cursor.c",
        "lib/src/tree_cursor.h",
        "lib/src/unicode.h",
    ],
    hdrs = [
        "lib/include/tree_sitter/api.h",
        "lib/include/tree_sitter/parser.h",
    ],
    copts = select({
        "@platforms//os:windows": WINDOWS_WARNING_SUPPRESSIONS,
        "//conditions:default": [],
    }),
    local_defines = ["_POSIX_C_SOURCE=200112L"],
    strip_include_prefix = "lib/include",
    visibility = ["//visibility:public"],
)