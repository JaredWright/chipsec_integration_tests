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
// Register one handler for an existing CPUID leaf. The handler behaves as
// follows:
//  - Increments a counter every time the handler runs
//  - Does not modify the vcpu in any way
//  - Returns false, yielding to the base cpuid handler
// 
// The counter is exposed through a seperate cpuid emulator at a non-existent
// cpuid leaf
//

static uint64_t g_counter = 0;

bool handler(vcpu_t *vcpu)
{
    counter++;
    return false;
}

bool emulator(vcpu_t *vcpu)
{
    cpuid::emulate(vcpu, g_counter, 0, 0, 0);
    vcpu->advance();
    return true;
}

bool vcpu_init(vcpu_t *vcpu)
{
    cpuid::add_handler(vcpu, 0x8086, cpuid::handler(handler));
    cpuid::add_emulator(vcpu, 0xF00D, cpuid::handler(emulator));

    return true;
}
