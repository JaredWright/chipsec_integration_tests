"""
A chipsec module to test the special Bareflank CPUID ACK leaf
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID ACK Test'
TAGS = ["BAREFLANK"]

class test_ack(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        self.load_vmm(module_argv, "bfvmm_static")

        result = self.cpuid(0x4BF00000, 0x0)
        expected = (0x4BF00001, 0, 0, 0)
        if not self.cpuid_check_result(result, expected):
            return ModuleResult.FAILED
        else:
            return ModuleResult.PASSED

        self.unload_vmm(module_argv)
