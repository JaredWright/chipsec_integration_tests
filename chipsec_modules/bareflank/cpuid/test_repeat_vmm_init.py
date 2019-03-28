"""
Test Scenario:

Run the special Bareflank "init" CPUID leaf repeatedly. Running this CPUID leaf
should not cause the system any instability

TODO: Test that this leaf does not re-initizlize the vCPU managing the core
that this test runs on
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *
import logging as pyLogging

_MODULE_NAME = 'CPUID Repeat Init Test'
TAGS = ["BAREFLANK"]

class test_repeat_vmm_init(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.load_vmm(module_argv, "vmm")

        # This test has very noisy output, silence to a reasonable level
        self.logger.rootLogger.setLevel(pyLogging.ERROR)

        for i in range(100):
            result = self.cpuid(0x4BF00010, 0x0)
            expected = (0x4BF00010, 0, 0, 0)
            if not self.cpuid_check_result(result, expected):
                self.logger.rootLogger.setLevel(self.logger.debug)
                self.logger.log_failed(_MODULE_NAME)
                return ModuleResult.FAILED

        self.logger.rootLogger.setLevel(self.logger.debug)
        self.logger.log_passed(_MODULE_NAME)
        self.unload_vmm(module_argv)

        return ModuleResult.PASSED
