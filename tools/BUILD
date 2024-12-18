package(default_visibility = ["//visibility:public"])

cc_library(
    name = "all",
    hdrs = ["all.h"],
    deps = [
        "//tools",
        "//tools/math",
    ],
)

cc_library(
    name = "tools",
    hdrs = [
        "io.h",
        "log.h",
        "main.h",
    ],
    srcs = [
        "io.cpp",
        "log.cpp",
        "main.cpp",
    ],
    deps = [
        "@abseil-cpp//absl/strings",
    ]
)

cc_test(
    name = "tools_test",
    srcs = [
        "io_test.cpp",
    ],
    deps = [
        "//tools",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)