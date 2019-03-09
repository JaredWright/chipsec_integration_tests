#
# Copyright (C) 2019 Assured Information Security, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.6)
project(chipsec_vmms C CXX)

include(${SOURCE_CMAKE_DIR}/project.cmake)
init_project()

add_vmm_executable(
    integration_cpuid_emulator_and_handler
    SOURCES cpuid/emulator_and_handler.cpp
)

add_vmm_executable(
    integration_cpuid_emulator_and_handler_exception
    SOURCES cpuid/emulator_and_handler_exception.cpp
)

add_vmm_executable(
    integration_cpuid_emulator_basic
    SOURCES cpuid/emulator_basic.cpp
)

add_vmm_executable(
    integration_cpuid_emulator_does_not_leak_hardware
    SOURCES cpuid/emulator_does_not_leak_hardware.cpp
)

add_vmm_executable(
    integration_cpuid_emulator_inputs_preserved
    SOURCES cpuid/emulator_inputs_preserved.cpp
)

add_vmm_executable(
    integration_cpuid_emulator_multiple
    SOURCES cpuid/emulator_multiple.cpp
)

add_vmm_executable(
    integration_cpuid_handler_basic
    SOURCES cpuid/handler_basic.cpp
)

add_vmm_executable(
    integration_cpuid_handler_inputs_preserved
    SOURCES cpuid/handler_inputs_preserved.cpp
)

add_vmm_executable(
    integration_cpuid_handler_multiple
    SOURCES cpuid/handler_multiple.cpp
)

add_vmm_executable(
    integration_cpuid_handler_vcpu_matches_hardware
    SOURCES cpuid/handler_vcpu_matches_hardware.cpp
)
