# Copyright 2019 The IREE Authors
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

load("@llvm-project//mlir:tblgen.bzl", "gentbl_cc_library")
load("//iree:build_defs.oss.bzl", "iree_cmake_extra_content")

package(
    default_visibility = ["//visibility:public"],
    features = ["layering_check"],
    licenses = ["notice"],  # Apache 2.0
)

gentbl_cc_library(
    name = "PassesIncGen",
    tbl_outs = [
        (
            ["-gen-pass-decls"],
            "Passes.h.inc",
        ),
    ],
    tblgen = "@llvm-project//mlir:mlir-tblgen",
    td_file = "Passes.td",
    deps = [
        "@llvm-project//mlir:PassBaseTdFiles",
    ],
)

cc_library(
    name = "Transforms",
    srcs = [
        "ConvertConv2D1x1ToMatmulPass.cpp",
        "ConvertConv2DToImg2ColPass.cpp",
        "ConvertLinalgMatmulToMmt4D.cpp",
        "ConvertToFlow.cpp",
        "DeduplicateExecutables.cpp",
        "DestructiveUpdateUtils.cpp",
        "DispatchLinalgOnTensors.cpp",
        "ExportBenchmarkFuncs.cpp",
        "FusionOfTensorOps.cpp",
        "InjectDispatchTracing.cpp",
        "InterchangeGenericOps.cpp",
        "OutlineDispatchRegions.cpp",
        "PadLinalgOps.cpp",
        "PadTensorToSubTensorInsert.cpp",
        "PassDetail.h",
        "Passes.cpp",
        "PromoteI1ToI8Pass.cpp",
        "StripAndSplatConstantVariables.cpp",
        "StripSignednessPass.cpp",
        "TypeConverter.cpp",
        "VerifyInputLegality.cpp",
    ],
    hdrs = [
        "DestructiveUpdateUtils.h",
        "Passes.h",
        "Passes.h.inc",
        "TypeConverter.h",
    ],
    deps = [
        ":PassesIncGen",
        "//iree/compiler/Dialect/Flow/Conversion/TensorToFlow",
        "//iree/compiler/Dialect/Flow/IR",
        "//iree/compiler/Dialect/HAL/IR",
        "//iree/compiler/Dialect/Util/IR",
        "//iree/compiler/Dialect/Util/Transforms",
        "//iree/compiler/Utils",
        "//llvm-external-projects/iree-dialects:IREELinalgExtDialect",
        "//llvm-external-projects/iree-dialects:IREELinalgExtTransforms",
        "@llvm-project//llvm:Support",
        "@llvm-project//mlir:Affine",
        "@llvm-project//mlir:DialectUtils",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:LinalgOps",
        "@llvm-project//mlir:LinalgTransforms",
        "@llvm-project//mlir:MemRefDialect",
        "@llvm-project//mlir:MemRefTransforms",
        "@llvm-project//mlir:Pass",
        "@llvm-project//mlir:SCFDialect",
        "@llvm-project//mlir:StandardOps",
        "@llvm-project//mlir:Support",
        "@llvm-project//mlir:TensorDialect",
        "@llvm-project//mlir:TosaDialect",
        "@llvm-project//mlir:TransformUtils",
        "@llvm-project//mlir:Transforms",
    ],
)

# TODO(#4919): For an unknown reason, GCC's devirtualization optimization wreaks
# havoc on this file. Needs to be further root caused. Seems to affect both 9.x
# and 10.x.
iree_cmake_extra_content(
    content = """
set_property(SOURCE
  DispatchLinalgOnTensors.cpp
  PROPERTY COMPILE_FLAGS $<$<CXX_COMPILER_ID:GNU>:-fno-devirtualize>)
""",
    inline = True,
)
