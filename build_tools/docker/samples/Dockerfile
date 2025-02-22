# Copyright 2021 The IREE Authors
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# An image for running IREE's samples. Includes support for:
#   * CMake
#   * Vulkan (using SwiftShader)
#   * Python (including `venv` and common pip packages needed for Colab)

FROM gcr.io/iree-oss/swiftshader@sha256:715bdde46dbf454c8b20b9f193ff5ca4b02c1e33a1bfbe83d60921aa21d64664

# Update setuptools per https://github.com/pypa/setuptools/issues/1694#issuecomment-466010982
RUN apt-get update \
  && apt-get install -y \
    python3-venv \
    python3.7-venv \
    python-setuptools \
  && python3 -m pip install --upgrade setuptools

# Install additional packages often used in notebooks.
# Installing these at the system level helps with caching, since venvs can
# set --system-site-packages to use the already installed versions.
#
# Note:
#   * Packages relating to TensorFlow are pinned to versions close to what
#     Colab includes in its hosted runtimes. We don't need to match all of
#     Colab's dependencies, but we should at least make an effort for the ones
#     our notebooks use.
#   * We explicitly do *not* install Jupyter notebook requirements since they
#     should be installed within venvs.
RUN python3 -m pip install --ignore-installed \
    numpy \
    matplotlib \
    bottleneck \
    tensorflow==2.5.0 \
    tensorflow_hub==0.12.0
