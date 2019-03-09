"""
Test Scenario:

A VMM emulates a CPUID leaf using multiple handlers. Each handler is
responsible for setting a single output register for the cpuid instruction
(i.e. one handler sets eax, another sets ebx, etc). The fourth handler returns
false, yielding to the base cpuid handler to complete any built-in behaviors.
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Multiple Handlers Test'
TAGS = ["BAREFLANK"]

class test_handler_multiple(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.load_vmm(module_argv, "integration_cpuid_handler_multiple_static")

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
