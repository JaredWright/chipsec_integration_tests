"""
Test Scenario:

Test for the existence of the special Bareflank CPUID ACK leaf
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID ACK Test'
TAGS = ["BAREFLANK"]

class test_ack(BareflankBaseModule):

    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.load_vmm(module_argv, "vmm")

        cpuid_result = self.cpuid(0x4BF00000, 0x0)
        expected = (0x4BF00001, 0, 0, 0)
        test_passed = self.cpuid_check_result(cpuid_result, expected)

        self.unload_vmm(module_argv)

        if not test_passed:
            return ModuleResult.FAILED
        else:
            return ModuleResult.PASSED
