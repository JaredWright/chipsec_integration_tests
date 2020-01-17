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
// Register one handler for an existing MSR. The handler behaves as follows:
//  - Increments a counter every time the handler runs
//  - Does not modify the vcpu in any way
//  - Returns false, yielding to the base rdmsr handler
// 
// The counter is exposed through a seperate msr emulator at a non-existent MSR
//

static uint32_t g_counter = 0;

bool handler(vcpu *vcpu)
{
    counter++;
    return false;
}

bool emulator(vcpu *vcpu)
{
    io_instruction::emulate(vcpu, g_counter);
    vcpu->advance();
    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    io_out::add_handler(vcpu, 0xCFC, io_out::handler(handler));
    io_in::add_emulator(vcpu, 0xCF8, io_in::handler(emulator));

    return true;
}
