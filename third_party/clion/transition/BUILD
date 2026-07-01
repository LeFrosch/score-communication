load("@aspect_bazel_lib//lib:transitions.bzl", "platform_transition_binary")
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = ["//score/mw/com"],
)

platform_transition_binary(
    name = "main_qnx",
    binary = ":main",
    target_platform = "@score_bazel_platforms//:x86_64-qnx-sdp_8.0.0-posix",
)

platform_transition_binary(
    name = "main_gcc",
    binary = ":main",
    target_platform = "@score_bazel_platforms//:x86_64-linux-gcc_12.2.0-posix",
)
