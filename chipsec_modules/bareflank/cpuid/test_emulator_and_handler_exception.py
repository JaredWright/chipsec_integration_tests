"""
A chipsec module to test Bareflank CPUID emulation throws an exception
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Emulator And Handler Exception Test'
TAGS = ["BAREFLANK"]

class test_emulator_and_handler_exception(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        self.load_vmm(module_argv, "integration_cpuid_emulator_and_handler_exception_static")

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
