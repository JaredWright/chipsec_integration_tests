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

bool emulator_1(vcpu *vcpu)
{
    vcpu->set_rax(0xBEEF);
    return false;
}

bool emulator_2(vcpu *vcpu)
{
    vcpu->set_rdx(0xF00D);
    vcpu->advance();
    return true;
}

bool emulator_3(vcpu *vcpu)
{
    vcpu->set_rax(0xDEAD);
    vcpu->set_rdx(0xDEAD);
    return false;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    vcpu->rdmsr_add_emulator(0x000000000000003B, handler_delegate_t::create<emulator_3>());
    vcpu->rdmsr_add_emulator(0x000000000000003B, handler_delegate_t::create<emulator_2>());
    vcpu->rdmsr_add_emulator(0x000000000000003B, handler_delegate_t::create<emulator_1>());
    vcpu->rdmsr_trap(0x000000000000003B);
    return true;
}
