load("@tree-sitter-format//tools:rules.bzl", "tsf_cc_binary", "tsf_cc_library")

tsf_cc_binary(
    name = "tree-sitter-format",
    srcs = ["main.cpp"],
    deps = [
        ":bracket_existance_traverser",
        ":indentation_traverser",
        ":parse_traverser",
        ":space_traverser",
        ":declaration_alignment_traverser",
        ":bitfield_alignment_traverser",
        ":assignment_alignment_traverser",
        ":initializer_list_alignment_traverser",
        ":constants",
        ":configuration",
        ":document",
        ":style",
        ":formatter",
        "@tree-sitter-cpp",
        "@yaml-cpp",
    ],
)

tsf_cc_library(
    name = "unicode_iterator",
    hdrs = ["document/UnicodeIterator.h"],
    srcs = ["document/UnicodeIterator.cpp"],
    include_prefix = "tree_sitter_format",
)

tsf_cc_library(
    name = "traverser",
    hdrs = ["traversers/Traverser.h"],
    srcs = ["traversers/Traverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":edits", ":document", ":style", "@tree-sitter"],
)

tsf_cc_library(
    name = "comment_reflow_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/CommentReflowTraverser.h"],
    srcs = ["traversers/CommentReflowTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "indentation_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/IndentationTraverser.h"],
    srcs = ["traversers/IndentationTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "bracket_existance_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/BracketExistanceTraverser.h"],
    srcs = ["traversers/BracketExistanceTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "space_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/SpaceTraverser.h"],
    srcs = ["traversers/SpaceTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "declaration_alignment_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/DeclarationAlignmentTraverser.h"],
    srcs = ["traversers/DeclarationAlignmentTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "bitfield_alignment_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/BitfieldAlignmentTraverser.h"],
    srcs = ["traversers/BitfieldAlignmentTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "assignment_alignment_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/AssignmentAlignmentTraverser.h"],
    srcs = ["traversers/AssignmentAlignmentTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "initializer_list_alignment_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/InitializerListAlignmentTraverser.h"],
    srcs = ["traversers/InitializerListAlignmentTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "parse_traverser",
    visibility = ["//visibility:public"],
    hdrs = ["traversers/ParseTraverser.h"],
    srcs = ["traversers/ParseTraverser.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":traverser",
        ":util",
    ],
)

tsf_cc_library(
    name = "constants",
    hdrs = ["Constants.h"],
    include_prefix = "tree_sitter_format",
    deps = ["@tree-sitter"],
)

tsf_cc_library(
    name = "util",
    hdrs = ["Util.h"],
    srcs = ["Util.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":constants",
        "@tree-sitter",
    ],
)

tsf_cc_library(
    name = "style",
    visibility = ["//visibility:public"],
    hdrs = ["style/Style.h"],
    srcs = ["style/Style.cpp"],
    include_prefix = "tree_sitter_format",
    deps = ["@yaml-cpp"],
)

tsf_cc_library(
    name = "configuration",
    hdrs = ["configuration/Configuration.h"],
    srcs = ["configuration/Configuration.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":style"],
)

tsf_cc_library(
    name = "position",
    hdrs = ["document/Position.h"],
    srcs = ["document/Position.cpp"],
    include_prefix = "tree_sitter_format",
    deps = ["@tree-sitter"],
)

tsf_cc_library(
    name = "range",
    hdrs = ["document/Range.h"],
    srcs = ["document/Range.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":position", "@tree-sitter"],
)

tsf_cc_library(
    name = "edits",
    hdrs = ["document/Edits.h"],
    srcs = ["document/Edits.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":position", ":range", "@tree-sitter"],
)

tsf_cc_library(
    name = "document_slice",
    hdrs = ["document/DocumentSlice.h"],
    srcs = ["document/DocumentSlice.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":unicode_iterator",
        ":position",
        ":range",
        "@tree-sitter"
    ],
)

tsf_cc_library(
    name = "document",
    visibility = ["//visibility:public"],
    hdrs = ["document/Document.h"],
    srcs = ["document/Document.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [
        ":document_slice",
        ":edits",
        ":position",
        ":range",
        "@tree-sitter",
        "@tree-sitter-cpp",
    ],
)

tsf_cc_library(
    name = "formatter",
    visibility = ["//visibility:public"],
    hdrs = ["Formatter.h"],
    srcs = ["Formatter.cpp"],
    include_prefix = "tree_sitter_format",
    deps = [":document", ":style", ":traverser"],
)