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
    // Get the high-level exit reason from within a VM exit handler
    vcpu->vmexit_reason();

    // Logging
    vcpu->log_debug();
    vcpu->log_info();
    vcpu->log_warning();
    vcpu->log_error();
    vcpu->dump();

    // General purpose register accessors
    vcpu->rax();      // ... etc
    vcpu->set_rax(0); // ... etc

    // Guest support
    vcpu->parent();
    vcpu->make_child();
    vcpu->clone();
    vcpu->migrate(0x0);
    vcpu->run();

    vcpu->hlt();
    vcpu->advance();

    return true;
}

bool vcpu_init_nonroot(vcpu *vcpu)
{
    vcpu->wrcr0_add_handler(handler);
    vcpu->wrcr3_add_handler(handler);
    vcpu->rdcr3_add_handler(handler);
    vcpu->cpuid_add_handler(handler);
    vcpu->rdmsr_add_handler(handler);
    vcpu->wrmsr_add_handler(handler);
    vcpu->ept_add_violation_handler(handler);
    vcpu->ept_misconfiguration_handler(handler);
    vcpu->io_add_in_handler(handler);
    vcpu->io_add_out_handler(handler);
    vcpu->xsetbv_add_handler(handler);
    vcpu->external_interrupt_add_handler(handler);
    vcpu->init_signal_add_handler(handler);

    vcpu->add_vmexit_handler();     // All VM exits (override base exit_handler)
    vcpu->run_add_handler(handler); // Runs on vcpu->run()
    vcpu->hlt_add_handler(handler); // Runs on vcpu->hlt()
    vcpu->launch_add_handler(handler);// Runs on vcpu->launch()
    vcpu->resume_add_handler(handler);// Runs on vcpu->resume()

    vcpu->enable_vpid();

    return true;
}
