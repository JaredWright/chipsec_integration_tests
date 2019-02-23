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
// Emulate a CPUID leaf using multiple handlers. Each handler is responsible for
// setting a single output register for the cpuid instruction (i.e. one handler
// sets eax, another sets ebx, etc). The fourth handler returns false, yielding
// to the base cpuid handler to complete any built-in behaviors.
//

bool handler_1(vcpu_t *vcpu)
{
    vcpu->set_rax(0xBEEF);
    return false;
}

bool handler_2(vcpu_t *vcpu)
{
    vcpu->set_rbx(0x5A);
    return false;
}

bool handler_3(vcpu_t *vcpu)
{
    vcpu->set_rcx(0xA55A);
    return false;
}

bool handler_4(vcpu_t *vcpu)
{
    vcpu->set_rdx(0xFFFFFFFF);
    return false;
}

bool vcpu_init(vcpu_t *vcpu)
{
    cpuid::add_handler(vcpu, 0xF00D, cpuid::handler(handler_4));
    cpuid::add_handler(vcpu, 0xF00D, cpuid::handler(handler_3));
    cpuid::add_handler(vcpu, 0xF00D, cpuid::handler(handler_2));
    cpuid::add_handler(vcpu, 0xF00D, cpuid::handler(handler_1));

    return true;
}
