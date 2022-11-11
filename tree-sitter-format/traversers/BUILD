load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_library")

tsf_cc_library(
    name = "traverser",
    hdrs = ["Traverser.h"],
    srcs = ["Traverser.cpp"],
    deps = [
        "//tree-sitter-format/document",
        "//tree-sitter-format/document:edits",
        "//tree-sitter-format/style",
        "@tree-sitter"
    ],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "comment_reflow_traverser",
    hdrs = ["CommentReflowTraverser.h"],
    srcs = ["CommentReflowTraverser.cpp"],
    deps = [
        "//tree-sitter-format:constants",
        "//tree-sitter-format/document:text_reflower",
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],
    
    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "indentation_traverser",
    hdrs = ["IndentationTraverser.h"],
    srcs = ["IndentationTraverser.cpp"],
    deps = [
        "//tree-sitter-format:constants",
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],

    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "bracket_existance_traverser",
    hdrs = ["BracketExistanceTraverser.h"],
    srcs = ["BracketExistanceTraverser.cpp"],
    deps = [
        "//tree-sitter-format:constants",
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],
    
    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "space_traverser",
    hdrs = ["SpaceTraverser.h"],
    srcs = ["SpaceTraverser.cpp"],
    deps = [
        "//tree-sitter-format:constants",
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],
    
    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "declaration_alignment_traverser",
    hdrs = ["DeclarationAlignmentTraverser.h"],
    srcs = ["DeclarationAlignmentTraverser.cpp"],
    deps = [
        "//tree-sitter-format:constants",
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],
    
    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "bitfield_alignment_traverser",
    hdrs = ["BitfieldAlignmentTraverser.h"],
    srcs = ["BitfieldAlignmentTraverser.cpp"],
    deps = [
        "//tree-sitter-format:constants",
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],
    
    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "assignment_alignment_traverser",
    hdrs = ["AssignmentAlignmentTraverser.h"],
    srcs = ["AssignmentAlignmentTraverser.cpp"],
    deps = [
        "//tree-sitter-format:constants",
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],
    
    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "initializer_list_alignment_traverser",
    hdrs = ["InitializerListAlignmentTraverser.h"],
    srcs = ["InitializerListAlignmentTraverser.cpp"],
    deps = [
        "//tree-sitter-format:constants",
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],
    
    visibility = ["//visibility:public"],
)

tsf_cc_library(
    name = "parse_traverser",
    hdrs = ["ParseTraverser.h"],
    srcs = ["ParseTraverser.cpp"],
    deps = [
        "//tree-sitter-format/traversers:traverser",
        "//tree-sitter-format:util",
    ],
    
    visibility = ["//visibility:public"],
)