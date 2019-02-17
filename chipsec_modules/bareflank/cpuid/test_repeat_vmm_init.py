"""
A chipsec module to test what happens when you run the special Bareflank "init"
CPUID leaf repeatedly. Running this CPUID leaf shouldn't cause the sysetem
any instability, and shouldn't re-initizlize the VMM (security concern)
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Repeat Init Integration Test'
TAGS = ["BAREFLANK"]

class test_repeat_vmm_init(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        self.load_vmm(module_argv, "bfvmm_static")

        for i in range(100):
            result = self.cpuid(0x4BF00010, 0x0)
            expected = (0x4BF00010, 0, 0, 0)
            if not self.cpuid_check_result(result, expected):
                return ModuleResult.FAILED

        return ModuleResult.PASSED

        self.unload_vmm(module_argv)
