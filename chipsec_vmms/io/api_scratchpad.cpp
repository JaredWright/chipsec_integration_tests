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
    vcpu->io_execute_in();  // pass-through the operation that caused the exit
    vcpu->io_emulate_in(0xFFFFFFFF); // masked to the size of the current io exit

    vcpu->io_execute_out();
    vcpu->io_emulate_out(0xFFFF);

    vcpu->io_vmexit_port_number();
    vcpu->io_vmexit_port_size();
    vcpu->io_vmexit_address();
    vcpu->io_vmexit_type();
    vcpu->io_vmexit_is_in();   // current VM exit caused by IN instruction variant
    vcpu->io_vmexit_is_out();  // current VM exit caused by OUT instruction variant
    vcpu->io_vmexit_is_read(); // current VM exit caused by MMIO read
    vcpu->io_vmexit_is_write();// current VM exit caused by MMIO write

    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    vcpu->io_add_out_handler(0xCF8, handler);
    vcpu->io_add_out_emulator(0xCF8, handler);
    vcpu->io_trap(0xCF8);

    vcpu->io_add_in_handler(0xCFC, handler);
    vcpu->io_add_in_emulator(0xCFC, handler);
    vcpu->io_trap(0xCFC);

    vcpu->io_vmexit_enable(0xCFC);
    vcpu->io_vmexit_disable(0xCFC);

    return true;
}
