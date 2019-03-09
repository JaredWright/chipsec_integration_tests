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
// Handle a CPUID leaf using multiple handlers. Each handler should be able
// to see the leaf and subleaf that caused the vmexit to happen, regardless of
// the values currently in vcpu->rax and vcpu->rcx. The leaf/subleaf read from
// each handler are exposed at a seperate emulated CPUID leaf for comparison
//

// Values for leaf/subleaf according to the expected vcpu general register
// locations (leaf = vcpu->gr1, subleaf = vcpu->gr2)
uint64_t leaf_internal_location = 0xBADC0FFE;
uint64_t subleaf_internal_location = 0xBADC0FFE;

// Values for leaf/subleaf as viewed by handler_1, calculated by the cpuid api
uint64_t leaf_at_handler_1 = 0xBADC0FFE;
uint64_t subleaf_at_handler_1 = 0xBADC0FFE;

// Values for leaf/subleaf as viewed by handler_2, calculated by the cpuid api
uint64_t leaf_at_handler_2 = 0xBADC0FFE;
uint64_t subleaf_at_handler_2 = 0xBADC0FFE;

bool handler_1(vcpu *vcpu)
{
    leaf_internal_location = vcpu->gr1();
    subleaf_internal_location = vcpu->gr2();

    leaf_at_handler_1 = cpuid::get_leaf(vcpu);
    subleaf_at_handler_1 = cpuid::get_subleaf(vcpu);

    vcpu->set_rax(0xFFFFFFFF);
    vcpu->set_rbx(0xFFFFFFFF);
    vcpu->set_rcx(0xFFFFFFFF);
    vcpu->set_rdx(0xFFFFFFFF);

    return false;
}

bool handler_2(vcpu *vcpu)
{
    leaf_at_handler_2 = cpuid::get_leaf(vcpu);
    subleaf_at_handler_2 = cpuid::get_subleaf(vcpu);

    vcpu->advance();
    return true;
}

bool emulator_3(vcpu *vcpu)
{
    vcpu->set_rax(leaf_internal_location);
    vcpu->set_rbx(0x0);
    vcpu->set_rcx(subleaf_internal_location);
    vcpu->set_rdx(0x0);

    vcpu->advance();
    return true;
}

bool emulator_4(vcpu *vcpu)
{
    vcpu->set_rax(leaf_at_handler_1);
    vcpu->set_rbx(0x0);
    vcpu->set_rcx(subleaf_at_handler_1);
    vcpu->set_rdx(0x0);

    vcpu->advance();
    return true;
}

bool emulator_5(vcpu *vcpu)
{
    vcpu->set_rax(leaf_at_handler_2);
    vcpu->set_rbx(0x0);
    vcpu->set_rcx(subleaf_at_handler_2);
    vcpu->set_rdx(0x0);

    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    cpuid::add_emulator(vcpu, 0xF00D, handler_delegate(handler_2));
    cpuid::add_emulator(vcpu, 0xF00D, handler_delegate(handler_1));

    cpuid::add_emulator(vcpu, 0xBEEF1, handler_delegate(emulator_3));
    cpuid::add_emulator(vcpu, 0xBEEF2, handler_delegate(emulator_4));
    cpuid::add_emulator(vcpu, 0xBEEF3, handler_delegate(emulator_5));

    return true;
}
