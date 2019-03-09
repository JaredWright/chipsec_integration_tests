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
// Register one emulator and one handler for the same CPUID leaf. The following
// behaviors should be observed:
//  - The emulator should be called first
//  - The emulator returns false, indicating that emulation is not complete
//  - An unhandled vmexit exception should occur
//  - The handler should never be called
//

bool emulator(vcpu *vcpu)
{ bfignored(vcpu); return false; }

bool handler(vcpu *vcpu)
{
    vcpu->set_rax(0xDEAD);
    return false;
}

void hlt_delegate(bfobject *obj)
{
    bfignored(obj);
    bfdebug_pass(0, "Halt delegate ran");
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    cpuid::add_emulator(vcpu, 0xF00D, handler_delegate(emulator));
    cpuid::add_handler(vcpu, 0xF00D, handler_delegate(handler));
    vcpu->add_hlt_delegate(hlt_delegate_t::create<hlt_delegate>());

    return true;
}
