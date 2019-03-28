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

cmake_minimum_required(VERSION 3.13)
project(chipsec_modules C CXX)

init_project(chipsec_modules INTERFACE)

file(GLOB_RECURSE CHIPSEC_MODULES_SRC_FILES "*/*.py")

set(CHIPSEC_MODULES_STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/stamp)
set(CHIPSEC_MODULES_STAMP ${CHIPSEC_MODULES_STAMP_DIR}/chipsec_output.stamp)
file(MAKE_DIRECTORY ${CHIPSEC_MODULES_STAMP_DIR})

add_custom_command(
    COMMAND cmake -E touch ${CHIPSEC_MODULES_STAMP}
    OUTPUT ${CHIPSEC_MODULES_STAMP}
    DEPENDS ${CHIPSEC_MODULES_SRC_FILES}
    COMMENT "Updating chipsec modules"
)

# add_custom_target(chipsec_modules ALL DEPENDS ${CHIPSEC_MODULES_STAMP})

install(DIRECTORY bareflank DESTINATION bin/chipsec_modules)

fini_project()
