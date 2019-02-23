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
//  - Register one emulator for a real CPUID leaf.
//  - The emulator executes a cpuid instruction manually for the leaf/subleaf
//      that caused the the vmexit
//  - The emulator saves the values reported by hardware, taking care not to set
//      any vcpu registers in the process
//  - The emulator returns true to end the handler chain.
//
//  The result of the real cpuid instruction is exposed through a seperate cpuid
//  emulator for comparison
//

static uint64_t g_rax = 0xBADC0FFE;
static uint64_t g_rbx = 0xBADC0FFE;
static uint64_t g_rcx = 0xBADC0FFE;
static uint64_t g_rdx = 0xBADC0FFE;

bool emulator_1(vcpu_t *vcpu)
{
    auto leaf = 0xBADC0FFE;     // TODO: look this up with whatever API is provided
    auto subleaf = 0xBADC0FFE;  // TODO: look this up with whatever API is provided
    auto ret = ::x64::cpuid::get(leaf, 0, subleaf, 0);

    g_rax = ret.rax;
    g_rbx = ret.rbx;
    g_rcx = ret.rcx;
    g_rdx = ret.rdx;

    vcpu->advance();
    return true;
}

bool emulator_2(vcpu_t *vcpu)
{
    cpuid::emulate(vcpu, g_rax, g_rbx, g_rcx, g_rdx);
    vcpu->advance();
    return true;
}

bool vcpu_init(vcpu_t *vcpu)
{
    cpuid::add_emulator(vcpu, 0x8086, cpuid::handler(emulator_1));
    cpuid::add_emulator(vcpu, 0xF00D, cpuid::handler(emulator_2));

    return true;
}
