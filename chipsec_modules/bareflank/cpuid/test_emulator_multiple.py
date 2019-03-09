"""
A chipsec module to test Bareflank CPUID emulation that is handled through
multiple emulators for the same CPUID leaf
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Multiple Emulators Test'
TAGS = ["BAREFLANK"]

class test_emulator_multiple(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        self.load_vmm(module_argv, "integration_cpuid_emulator_multiple_static")

        cpuid_result = self.cpuid(0xF00D, 0x0)
        expected = (0xBEEF, 0xA55A, 0x5AA5AA55, 0xFFFFFFFF)
        test_passed = self.cpuid_check_result(cpuid_result, expected)

        self.unload_vmm(module_argv)

        if test_passed == True:
            self.logger.log_passed(_MODULE_NAME)
            return ModuleResult.PASSED
        else:
            self.logger.log_failed(_MODULE_NAME)
            return ModuleResult.FAILED
