# Copyright (c) 2025 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(AS289R2 INTERFACE)

target_include_directories(AS289R2
    INTERFACE
        .
)

target_sources(AS289R2
    INTERFACE
        AS289R2.cpp
)
