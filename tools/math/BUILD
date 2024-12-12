package(default_visibility = ["//visibility:public"])

cc_library(
    name = "math",
    hdrs = ["vec2.h"],
    srcs = ["vec2.cpp"],
)

cc_test(
    name = "math_tests",
    srcs = [
        "vec2_test.cpp",
    ],
    deps = [
        "//tools/math:math",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)