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
// Make sure that the vcpu passed into a cpuid handler already contains the
// values reported by hardware for the cpuid instruction that caused the vmexit.
// A single cpuid handler is registered for an existing cpuid leaf, and behaves
// as follows:
//
//  - Save the vcpu state associated with cpuid instruction "outputs"
//      (i.e. eax, ebx, ecx, edx).
//  - Re-run CPUID manually and re-populate the vcpu with hardware state.
//      This should result in the vcpu's registers retaining the same values 
//  - Return true to end the handler chain
// 
// The "original" vcpu state (at the handler's entry point) is exposed through a
// sperate cpuid emulator for comparison
//

uint64_t g_rax = 0;
uint64_t g_rbx = 0;
uint64_t g_rcx = 0;
uint64_t g_rdx = 0;

bool handler(vcpu *vcpu)
{
    g_rax = vcpu->rax();
    g_rbx = vcpu->rbx();
    g_rcx = vcpu->rcx();
    g_rdx = vcpu->rdx();

    vcpu->set_rax(cpuid::get_leaf(vcpu));
    vcpu->set_rbx(0);
    vcpu->set_rcx(cpuid::get_subleaf(vcpu));
    vcpu->set_rdx(0);
    cpuid::execute(vcpu);

    vcpu->advance();
    return true;
}

bool emulator(vcpu *vcpu)
{
    vcpu->set_rax(g_rax);
    vcpu->set_rbx(g_rbx);
    vcpu->set_rcx(g_rcx);
    vcpu->set_rdx(g_rdx);

    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    cpuid::add_handler(vcpu, 0x0, handler_delegate(handler));
    cpuid::add_emulator(vcpu, 0xF00D, handler_delegate(emulator));

    return true;
}
