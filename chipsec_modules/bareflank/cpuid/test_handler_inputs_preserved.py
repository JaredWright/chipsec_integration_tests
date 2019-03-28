"""
Test Scenario:

A VMM handles a CPUID leaf using multiple handlers. Each handler should be able
to see the leaf and subleaf that caused the vmexit to happen, regardless of
the values currently in vcpu->rax and vcpu->rcx. The leaf/subleaf read from
each handler are exposed at a seperate emulated CPUID leaf for comparison
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Handler Preserves Inputs'
TAGS = ["BAREFLANK"]

class test_handler_inputs_preserved(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        test_passed = True

        self.load_vmm(module_argv, "integration_cpuid_handler_inputs_preserved")

        # Trigger the test to happen
        self.cpuid(0xF00D, 0xBEEF)

        # Check the observed leaf/subleaf values at all three locations
        internals_cpuid_result = self.cpuid(0xBEEF1, 0x0)
        emulator1_cpuid_result = self.cpuid(0xBEEF2, 0x0)
        emulator2_cpuid_result = self.cpuid(0xBEEF3, 0x0)

        self.unload_vmm(module_argv)

        # ALL THREE results should match
        if not self.cpuid_check_result(internals_cpuid_result, emulator1_cpuid_result):
            msg = "Leaf/subleaf does not match between vcpu general registers "
            msg += "and CPUID API at emulator_1"
            self.logger.log_failed(msg)
            test_passed = False;

        if not self.cpuid_check_result(emulator1_cpuid_result, emulator2_cpuid_result):
            msg = "Leaf/subleaf does not match between emulator1 and emulator2 "
            msg += "using the CPUID API"
            self.logger.log_failed(msg)
            test_passed = False;

        if test_passed == True:
            self.logger.log_passed(_MODULE_NAME)
            return ModuleResult.PASSED
        else:
            self.logger.log_failed(_MODULE_NAME)
            return ModuleResult.FAILED
