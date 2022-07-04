workspace(name = "tree-sitter-format")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

##################################
# Setup skylib convenience rules #
##################################

# Catch2 requires the expand_template macros which as of 2022-07-02 is not yet in a tagged release
http_archive(
    name = "bazel_skylib",
    sha256 = "b8fbb746f4525d180f2fe0fdbe86c85fe8f6809ff9b12c3bcb83b7455dc77778",
    strip_prefix = "bazel-skylib-a501641daebdce8601ee6bbc6b5d177c688d3517",
    urls = ["https://github.com/bazelbuild/bazel-skylib/archive/a501641daebdce8601ee6bbc6b5d177c688d3517.zip"],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

#####################
# Setup tree-sitter #
#####################

http_archive(
    name = "tree-sitter",
    build_file = "@tree-sitter-format//:third_party/tree-sitter.BUILD",
    sha256 = "5327b7115d60482f3241324c075a7430c1cb5f3a5826f48d171631a27e60c3aa",
    strip_prefix = "tree-sitter-0.20.6",
    urls = ["https://github.com/tree-sitter/tree-sitter/archive/refs/tags/v0.20.6.zip"],
)

http_archive(
    name = "tree-sitter-cpp",
    build_file = "@tree-sitter-format//:third_party/tree-sitter-cpp.BUILD",
    sha256 = "105d79c1ac2422428339a4e8637b290b2f119483da5095b7023e4aa14128c45b",
    strip_prefix = "tree-sitter-cpp-38d8b495bd43977498f0eb122e0f9cfef8526d18",
    url = "https://github.com/tree-sitter/tree-sitter-cpp/archive/38d8b495bd43977498f0eb122e0f9cfef8526d18.zip",
)

################
# Setup catch2 #
################

http_archive(
    name = "catch2",
    sha256 = "809789b8fd1083cee29baacd3b6da32c93d18a069edf714c704811a4ead67472",
    strip_prefix = "Catch2-3.0.1",
    url = "https://github.com/catchorg/Catch2/archive/refs/tags/v3.0.1.zip",
)

##################
# Setup yaml-cpp #
##################

http_archive(
    name = "yaml-cpp",
    sha256 = "43e6a9fcb146ad871515f0d0873947e5d497a1c9c60c58cb102a97b47208b7c3",
    strip_prefix = "yaml-cpp-yaml-cpp-0.7.0",
    url = "https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.7.0.tar.gz"
)
