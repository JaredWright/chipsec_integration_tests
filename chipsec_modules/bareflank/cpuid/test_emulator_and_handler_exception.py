"""
Test Scenario:

A VMM registers one emulator and one handler for the same CPUID leaf. The =
following behaviors should be observed:
  - The emulator should be called first
  - The emulator returns false, indicating that emulation is not complete
  - An unhandled vmexit exception should occur
  - The handler should never be called
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Emulator And Handler Exception Test'
TAGS = ["BAREFLANK"]

class test_emulator_and_handler_exception(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.load_vmm(module_argv, "integration_cpuid_emulator_and_handler_exception")

        cpuid_result = self.cpuid(0xF00D, 0)
        expected = (0, 0, 0, 0)
        test_passed = self.cpuid_check_result(cpuid_result, expected)

        self.unload_vmm(module_argv)

        if test_passed == True:
            self.logger.log_passed(_MODULE_NAME)
            return ModuleResult.PASSED
        else:
            self.logger.log_failed(_MODULE_NAME)
            return ModuleResult.FAILED
