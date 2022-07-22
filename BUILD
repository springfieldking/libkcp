cc_library(
    name = "ikcp",
    srcs = [
        "ikcp.c",
    ],
    hdrs = [
        "ikcp.h",
    ],
)

cc_library(
    name = "galois",
    srcs = [
        "galois.cpp",
        "galois_noasm.cpp",
        "galois_table.c",
    ],
    hdrs = [
        "galois.h",
        "galois_noasm.h",
    ],
)

cc_library(
    name = "matrix",
    srcs = [
        "matrix.cpp",
    ],
    hdrs = [
        "matrix.h",
    ],
    deps = [
        "galois",
    ],
)

cc_test(
    name = "matrix_test",
    srcs = [
        "matrix_test.cc",
    ],
    deps = [
        "matrix",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "reedsolomon",
    srcs = [
        "reedsolomon.cpp",
    ],
    hdrs = [
        "reedsolomon.h",
    ],
    deps = [
        "matrix",
        "galois",
    ],
)

cc_library(
    name = "fec",
    srcs = [
        "fec.cpp",
    ],
    hdrs = [
        "fec.h",
        "encoding.h",
    ],
    deps = [
        "reedsolomon",
    ],
)

cc_library(
    name = "sess",
    srcs = [
        "sess.cpp",
    ],
    hdrs = [
        "sess.h",
    ],
    deps = [
        "fec",
        "ikcp",
    ],
)

cc_library(
    name = "libkcp",
    deps = [
        "sess",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "fec_test",
    srcs = [
        "fec_test.h",
        "fec_test.cc",
    ],
    deps = [
        "libkcp",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_binary(
    name = "fec_benchmark",
    srcs = [
        "fec_test.h",
        "fec_benchmark.cc",
    ],
    deps = [
        "libkcp",
        "@com_github_google_benchmark//:benchmark",
        "@com_github_google_benchmark//:benchmark_main",
    ],
)