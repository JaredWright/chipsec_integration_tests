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

uint64_t g_rax = 0;
uint64_t g_rdx = 0;

bool handler(vcpu *vcpu)
{
    g_rax = vcpu->rax();
    g_rdx = vcpu->rdx();

    vcpu->set_rax(vcpu->rdmsr_vmexit_address());
    vcpu->rdmsr_execute();

    vcpu->advance();
    return true;
}

bool emulator(vcpu *vcpu)
{
    vcpu->set_rax(g_rax);
    vcpu->set_rdx(g_rdx);

    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    vcpu->rdmsr_add_handler(0x000000000000003B, handler_delegate_t::create<handler>());
    vcpu->rdmsr_trap(0x000000000000003B);

    vcpu->rdmsr_add_emulator(0x000000000000F00D, handler_delegate_t::create<emulator>());
    vcpu->rdmsr_trap(0x000000000000F00D);

    return true;
}
