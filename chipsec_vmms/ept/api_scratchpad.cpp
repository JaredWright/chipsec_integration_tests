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
    vcpu->ept_vmexit_gpa();
    vcpu->ept_vmexit_gva();
    vcpu->ept_vmexit_is_read();
    vcpu->ept_vmexit_is_write();
    vcpu->ept_vmexit_is_execute();
    vcpu->ept_vmexit_is_misconfiguration();
    vcpu->ept_vmexit_is_violation();

    vcpu->ept_map_4k(0xF00D, 0xBEEF);
    vcpu->ept_map_2m(0xF00D, 0xBEEF);
    vcpu->ept_map_1g(0xF00D, 0xBEEF);
    vcpu->ept_unmap(0xF00D);
    vcpu->ept_set_mmap(mmap);
    vcpu->ept_enable();
    vcpu->ept_disable();

    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    vcpu->ept_add_misconfiguration_handler(handler);
    vcpu->ept_add_violation_handler(handler);

    return true;
}
