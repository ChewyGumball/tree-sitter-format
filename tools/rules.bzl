load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

def tsf_cc_binary(name, copts = None, **kwargs):
    cc_binary(
        name = name,
        copts = select({
            "@platforms//os:windows": ["-std:c++20", "-Zc:__cplusplus", "-WX", "-W4"],
            "//conditions:default": ["-std=c++20"],
        }) + (copts or []),
        **kwargs
    )

def tsf_cc_library(name, copts = None, **kwargs):
    cc_library(
        name = name,
        copts = select({
            "@platforms//os:windows": ["-std:c++20", "-Zc:__cplusplus", "-WX", "-W4"],
            "//conditions:default": ["-std=c++20"],
        }) + (copts or []),
        **kwargs
    )

def tsf_cc_test(name, deps = None, copts = None, **kwargs):
    cc_test(
        name = name,
        deps = ["//tools:catch2_test_main"] + (deps or []),
        copts = select({
            "@platforms//os:windows": ["-std:c++20", "-Zc:__cplusplus", "-WX", "-W4"],
            "//conditions:default": ["-std=c++20"],
        }) + (copts or []),
        **kwargs
    )