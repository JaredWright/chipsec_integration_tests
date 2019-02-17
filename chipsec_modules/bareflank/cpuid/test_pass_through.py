"""
A chipsec module to test Bareflank CPUID pass-through
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Pass Through Test'
TAGS = ["BAREFLANK"]

class test_pass_through(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        self.load_vmm(module_argv, "integration_cpuid_pass_through_static")

        cpuid_result = self.cpuid(0x8086, 0x0)
        expected = self.cpuid(0xF00D, 0x0)
        test_passed = self.cpuid_check_result(cpuid_result, expected)

        self.unload_vmm(module_argv)

        if not test_passed:
            return ModuleResult.FAILED
        else:
            return ModuleResult.PASSED
