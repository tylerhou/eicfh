load("@rules_foreign_cc//foreign_cc:defs.bzl", "make")

filegroup(
    name = "Halide_srcs",
    srcs = glob(["Halide/**"]),
)

make(
    name = "Halide",
    copts = ["-stdlib=libstdc++"],
    env = {
        "LLVM_CONFIG": "/usr/bin/llvm-config-18",
        "CXX": "clang++-18",
        "CC": "clang-18",
    },
    lib_source = ":Halide_srcs",
    linkopts = ["-ltinfo", "-lz", "-lzstd"],
    out_shared_libs = ["libHalide.so"],
    out_static_libs = ["libHalide.a"],
    visibility = ["//visibility:public"],
)
