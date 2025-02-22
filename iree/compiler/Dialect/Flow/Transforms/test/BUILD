# Copyright 2019 The IREE Authors
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

load("//iree:lit_test.bzl", "iree_lit_test_suite")
load("//build_tools/bazel:enforce_glob.bzl", "enforce_glob")

package(
    default_visibility = ["//visibility:public"],
    features = ["layering_check"],
    licenses = ["notice"],  # Apache 2.0
)

iree_lit_test_suite(
    name = "lit",
    srcs = enforce_glob(
        [
            "conv1x1_to_matmul.mlir",
            "conv2d_to_img2col.mlir",
            "convert_linalg_tensor_ops_after.mlir",
            "convert_linalg_tensor_ops_before.mlir",
            "deduplicate_executables.mlir",
            "dispatch_linalg_on_tensors.mlir",
            "dispatch_linalg_on_tensors_elementwise.mlir",
            "dispatch_linalg_on_tensors_fusion.mlir",
            "export_benchmark_funcs.mlir",
            "inject_dispatch_tracing.mlir",
            "interchange_generic_ops.mlir",
            "matmul_to_mmt4d.mlir",
            "outline_dispatch_regions.mlir",
            "pad_linalg_ops.mlir",
            "pad_tensor_to_tensor.mlir",
            "promote_i1_to_i8.mlir",
            "strip_and_splat_constant_variables.mlir",
            "strip_signedness.mlir",
            "transformation.mlir",
            "verify_input_ir.mlir",
        ],
        include = ["*.mlir"],
    ),
    data = [
        "//iree/tools:IreeFileCheck",
        "//iree/tools:iree-opt",
    ],
)
