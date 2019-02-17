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

include(${CMAKE_CURRENT_LIST_DIR}/targets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/macros.cmake)

include_dependency(CMAKE_CURRENT_LIST_DIR chipsec)

userspace_extension(
    chipsec_modules
    DEPENDS chipsec
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/chipsec
)

vmm_extension(
    chipsec_vmms
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src
)

chipsec_test(cpuid_emulation
    VMM_EXECUTABLE cpuid_emulation_test
    CHIPSEC_MODULE bareflank.cpuid.test_emulation
    VERBOSE
)

chipsec_test(cpuid_ack
    VMM_EXECUTABLE bfvmm
    CHIPSEC_MODULE bareflank.cpuid.test_ack
)
