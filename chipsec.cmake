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

message(STATUS "Including dependency: chipsec")

find_package(Python2 COMPONENTS Interpreter REQUIRED)

set(CHIPSEC_SOURCE_DIR ${CACHE_DIR}/chipsec)
set(CHIPSEC_BUILD_DIR ${DEPENDS_DIR}/chipsec/${USERSPACE_PREFIX}/build)
set(CHIPSEC_INSTALL_DIR ${USERSPACE_PREFIX_PATH}/bin/chipsec)

set(CHIPSEC_URL "https://github.com/chipsec/chipsec/archive/v1.3.6.zip")
set(CHIPSEC_URL_MD5 "f2177858792f21fbc693b01dea157b6a")

download_dependency(
    chipsec
    URL         ${CHIPSEC_URL}
    URL_MD5     ${CHIPSEC_URL_MD5}
)

add_dependency(
    chipsec userspace
    CONFIGURE_COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CHIPSEC_SOURCE_DIR} ${CHIPSEC_BUILD_DIR}
    BUILD_COMMAND
        ${Python2_EXECUTABLE} setup.py build_ext -i
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CHIPSEC_BUILD_DIR} ${CHIPSEC_INSTALL_DIR}
)
