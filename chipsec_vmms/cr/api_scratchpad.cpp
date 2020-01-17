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

bool handler(vcpu *vcpu)
{
    vcpu->wrcr0_vmexit_value();
    vcpu->wrcr0_execute();

    vcpu->wrcr3_vmexit_value();
    vcpu->wrcr3_execute();

    vcpu->rdcr3_vmexit_value();
    vcpu->rdcr3_execute();
    vcpu->rdcr3_emulate(0xFFFFFFFF);

    vcpu->wrcr4_vmexit_value();
    vcpu->wrcr4_execute();

    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    vcpu->wrcr0_add_handler(handler);
    vcpu->wrcr0_add_emulator(handler);
    vcpu->wrcr0_vmexit_enable();
    vcpu->wrcr0_vmexit_disable();

    vcpu->wrcr3_add_handler(handler);
    vcpu->wrcr3_add_emulator(handler);
    vcpu->wrcr3_vmexit_enable();
    vcpu->wrcr3_vmexit_disable();

    vcpu->rdcr3_add_handler(handler);
    vcpu->rdcr3_add_emulator(handler);
    vcpu->rdcr3_vmexit_enable();
    vcpu->rdcr3_vmexit_disable();

    return true;
}
