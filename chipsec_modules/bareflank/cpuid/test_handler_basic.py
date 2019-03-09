"""
A chipsec module to test a basic Bareflank CPUID handler
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Handler Basic'
TAGS = ["BAREFLANK"]

class test_handler_basic(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        test_passed = True

        # CPUID leaf when no vmm is loaded. Conviniently, CPUID leaf 0 always
        # has outputs for eax, ebx, ecx, and edx
        real_cpuid_result = self.cpuid(0, 0)

        self.load_vmm(module_argv, "integration_cpuid_handler_basic_static")

        # Read the same CPUID leaf when vmm is loaded and emulating
        handled_cpuid_result = self.cpuid(0, 0)
        self.cpuid_check_result(real_cpuid_result, handled_cpuid_result)

        # Check the emulated CPUID counter leaf to make sure the handler ran
        emulated_cpuid_result = self.cpuid(0xF00D, 0)

        # Check that the counter indicates that the CPUID handler ran once
        if not self.cpuid_check_result(emulated_cpuid_result, (1, 0, 0, 0)):
            self.logger.log_failed("Counter leaf returned unexpected value")
            test_passed = False;

        # Check that the counter increments correctly a few times
        for i in range(2, 5):
            self.cpuid(0, 0)
            emulated_cpuid_result = self.cpuid(0xF00D, 0)
            if not self.cpuid_check_result(emulated_cpuid_result, (i, 0, 0, 0)):
                self.logger.log_failed("Counter leaf returned unexpected value")
                test_passed = False;

        self.unload_vmm(module_argv)

        if test_passed == True:
            self.logger.log_passed(_MODULE_NAME)
            return ModuleResult.PASSED
        else:
            self.logger.log_failed(_MODULE_NAME)
            return ModuleResult.FAILED
