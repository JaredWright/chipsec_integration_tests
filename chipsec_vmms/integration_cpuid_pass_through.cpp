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

static uint64_t g_rax = 0;
static uint64_t g_rbx = 0;
static uint64_t g_rcx = 0;
static uint64_t g_rdx = 0;

bool test_pass_through_handler(vcpu_t vcpu, cpuid::info_t &info)
{
    bfignored(info);

    cpuid::pass_through(vcpu);

    g_rax = vcpu->rax();
    g_rbx = vcpu->rbx();
    g_rcx = vcpu->rcx();
    g_rdx = vcpu->rdx();

    return true;
}

bool test_value_matches_handler(vcpu_t vcpu, cpuid::info_t &info)
{
    bfignored(info);

    cpuid::emulate(vcpu, g_rax, g_rbx, g_rcx, g_rdx);

    return true;
}

bool vmm_main(vcpu_t vcpu)
{
    // Pass through leaf 0x8086
    auto handler = cpuid::handler(test_pass_through_handler);
    cpuid::handle(vcpu, 0x8086, handler);

    // Expose the value that was supposed to be passed through at leaf 0xF00D
    handler = cpuid::handler(test_value_matches_handler);
    cpuid::handle(vcpu, 0xF00D, handler);

    return true;
}
