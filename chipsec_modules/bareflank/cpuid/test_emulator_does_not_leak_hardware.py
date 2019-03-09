"""
Test Scenario:

Make sure that the vcpu passed into a cpuid emulator does NOT contain the
values reported by hardware for the cpuid instruction that caused the vmexit.
This tests that a cpuid emulator does not transparently leak values from
hardware without explicitly trying to do so.

To test this property, a VMM is loaded that:
  - Registers one emulator for a real CPUID leaf (0 = Basic CPUID Information)
  - The emulator does nothing, and returns true to end the handler chain.

The observer of the emulated CPUID leaf should not see any values returned
through eax, ebx, ecx, or edx.
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Emulator Does Not Leak Hardware'
TAGS = ["BAREFLANK"]

class test_emulator_does_not_leak_hardware(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        test_passed = True

        # CPUID leaf when no vmm is loaded. Conviniently, CPUID leaf 0 always
        # has outputs for eax, ebx, ecx, and edx
        real_cpuid_result = self.cpuid(0, 0)
        self.cpuid_print(real_cpuid_result)

        self.load_vmm(module_argv, "integration_cpuid_emulator_does_not_leak_hardware_static")

        # Read the same CPUID leaf when vmm is loaded and emulating
        emulated_cpuid_result = self.cpuid(0, 0)
        self.cpuid_print(emulated_cpuid_result)

        self.unload_vmm(module_argv)

        if emulated_cpuid_result[0] == real_cpuid_result[0]:
            self.logger.log_failed("Emulated CPUID leaf leaks eax from hardware")
            test_passed = False;

        if emulated_cpuid_result[1] == real_cpuid_result[1]:
            self.logger.log_failed("Emulated CPUID leaf leaks ebx from hardware")
            test_passed = False;

        if emulated_cpuid_result[2] == real_cpuid_result[2]:
            self.logger.log_failed("Emulated CPUID leaf leaks ecx from hardware")
            test_passed = False;

        if emulated_cpuid_result[3] == real_cpuid_result[3]:
            self.logger.log_failed("Emulated CPUID leaf leaks edx from hardware")
            test_passed = False;

        if test_passed == True:
            self.logger.log_passed(_MODULE_NAME)
            return ModuleResult.PASSED
        else:
            self.logger.log_failed(_MODULE_NAME)
            return ModuleResult.FAILED
