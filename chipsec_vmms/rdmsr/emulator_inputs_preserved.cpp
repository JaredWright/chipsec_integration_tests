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

// Value for MSR address according to the expected vcpu general register
// locations (address = vcpu->gr1)
uint64_t msr_address_internal_location = 0xBADC0FFE;

// Values for MSR address as viewed by emulator_1, calculated by the rdmsr api
uint64_t msr_address_at_emulator_1 = 0xBADC0FFE;

// Values for MSR address as viewed by emulator_2, calculated by the rdmsr api
uint64_t msr_address_at_emulator_2 = 0xBADC0FFE;

bool emulator_1(vcpu *vcpu)
{
    msr_address_internal_location = vcpu->gr1();
    msr_address_at_emulator_1 = vcpu->rdmsr_vmexit_address();

    vcpu->set_rax(0xFFFFFFFF);
    vcpu->set_rdx(0xFFFFFFFF);

    return false;
}

bool emulator_2(vcpu *vcpu)
{
    msr_address_at_emulator_2 = vcpu->rdmsr_vmexit_address();

    vcpu->advance();
    return true;
}

bool emulator_3(vcpu *vcpu)
{
    vcpu->rdmsr_emulate(msr_address_internal_location);

    vcpu->advance();
    return true;
}

bool emulator_4(vcpu *vcpu)
{
    vcpu->rdmsr_emulate(msr_address_at_emulator_1);

    vcpu->advance();
    return true;
}

bool emulator_5(vcpu *vcpu)
{
    vcpu->rdmsr_emulate(msr_address_at_emulator_2);

    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    vcpu->rdmsr_add_emulator(0x000000000000F00D, handler_delegate_t::create<emulator_2>());
    vcpu->rdmsr_add_emulator(0x000000000000F00D, handler_delegate_t::create<emulator_1>());
    vcpu->rdmsr_trap(0x000000000000F00D);

    vcpu->rdmsr_add_emulator(0x00000000000BEEF1, handler_delegate_t::create<emulator_3>());
    vcpu->rdmsr_trap(0x00000000000BEEF1);

    vcpu->rdmsr_add_emulator(0x00000000000BEEF2, handler_delegate_t::create<emulator_4>());
    vcpu->rdmsr_trap(0x00000000000BEEF2);

    vcpu->rdmsr_add_emulator(0x00000000000BEEF3, handler_delegate_t::create<emulator_5>());
    vcpu->rdmsr_trap(0x00000000000BEEF3);

    return true;
}
