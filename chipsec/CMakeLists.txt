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
project(cpuid_chipsec_modules C CXX)

include(${SOURCE_CMAKE_DIR}/project.cmake)
init_project()

file(GLOB_RECURSE CHIPSEC_SRC_FILES "modules/*.py")

set(CHIPSEC_STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/stamp)
set(CHIPSEC_OUTPUT_STAMP ${CHIPSEC_STAMP_DIR}/chipsec_output.stamp)
file(MAKE_DIRECTORY ${CHIPSEC_STAMP_DIR})

add_custom_command(
    COMMAND cmake -E touch ${CHIPSEC_OUTPUT_STAMP}
    OUTPUT ${CHIPSEC_OUTPUT_STAMP}
    DEPENDS ${CHIPSEC_SRC_FILES}
    COMMENT "Updating chipsec modules"
)

add_custom_target(chipsec_modules ALL DEPENDS ${CHIPSEC_OUTPUT_STAMP})

install(DIRECTORY modules/ DESTINATION bin/chipsec/chipsec/modules)
