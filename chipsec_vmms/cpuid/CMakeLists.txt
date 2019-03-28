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

chipsec_test_vmm(
    integration_cpuid_emulator_and_handler
    SOURCES emulator_and_handler.cpp
)

chipsec_test_vmm(
    integration_cpuid_emulator_and_handler_exception
    SOURCES emulator_and_handler_exception.cpp
)


chipsec_test_vmm(
    integration_cpuid_emulator_basic
    SOURCES emulator_basic.cpp
)

chipsec_test_vmm(
    integration_cpuid_emulator_does_not_leak_hardware
    SOURCES emulator_does_not_leak_hardware.cpp
)

chipsec_test_vmm(
    integration_cpuid_emulator_inputs_preserved
    SOURCES emulator_inputs_preserved.cpp
)

chipsec_test_vmm(
    integration_cpuid_emulator_multiple
    SOURCES emulator_multiple.cpp
)

chipsec_test_vmm(
    integration_cpuid_handler_basic
    SOURCES handler_basic.cpp
)

chipsec_test_vmm(
    integration_cpuid_handler_inputs_preserved
    SOURCES handler_inputs_preserved.cpp
)

chipsec_test_vmm(
    integration_cpuid_handler_multiple
    SOURCES handler_multiple.cpp
)

chipsec_test_vmm(
    integration_cpuid_handler_vcpu_matches_hardware
    SOURCES handler_vcpu_matches_hardware.cpp
)
