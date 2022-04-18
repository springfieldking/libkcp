cc_library(
    name = "libkcp",
    srcs = [
        "ikcp.c",
        "sess.cpp",
        "galois.cpp",
        "galois_noasm.cpp",
        "matrix.cpp",
        "inversion_tree.cpp",
        "reedsolomon.cpp",
        "fec.cpp",
        "galois_table.c",
    ],
    hdrs = [
        "ikcp.h",
        "sess.h",
        "galois.h",
        "galois_noasm.h",
        "matrix.h",
        "inversion_tree.h",
        "reedsolomon.h",
        "fec.h",
        "encoding.h",
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