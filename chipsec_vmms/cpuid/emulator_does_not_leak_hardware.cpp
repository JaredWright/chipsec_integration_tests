//
// Copyright (C) 2019 Assured Information Security, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <vmm.h>

using namespace bfvmm::intel_x64;

// Scenario:
//
// Make sure that the vcpu passed into a cpuid emulator does NOT contain the
// values reported by hardware for the cpuid instruction that caused the vmexit.
// This tests that a cpuid emulator does not transparently leak values from
// hardware without explicitly trying to do so. To test this property:
//
//  - Register one emulator for a real CPUID leaf (0 = Basic CPUID Information)
//  - The emulator does nothing, and returns true to end the handler chain.
//
//  The observer of the emulated CPUID leaf should not see any values returned
//  through eax, ebx, ecx, or edx.
//

bool emulator_1(vcpu *vcpu)
{
    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    cpuid::add_emulator(vcpu, 0, handler_delegate(emulator_1));
    return true;
}
