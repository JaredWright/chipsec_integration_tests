"""
A chipsec module to test Bareflank CPUID emulation
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Emulation Test'
TAGS = ["BAREFLANK"]

class test_emulation(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

        # Sets to True if any test case fails
        self._module_failed = False

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        self.load_vmm(module_argv, "integration_cpuid_emulation_static")

        self._test_zeros_preserved()
        self._test_ones_preserved()
        self._test_register_mask()

        self.unload_vmm(module_argv)

        if self._module_failed == True:
            self.logger.log_failed(_MODULE_NAME)
            return ModuleResult.FAILED
        else:
            self.logger.log_passed(_MODULE_NAME)
            return ModuleResult.PASSED

    def _test_zeros_preserved(self):
        result = self.cpuid(0xF00D, 0x0)
        expected = (0xBADC0FFE, 0, 0, 0)
        if not self.cpuid_check_result(result, expected):
            self._module_failed = True

    def _test_ones_preserved(self):
        result = self.cpuid(0xF00D, 0xFFFFFFFF)
        expected = (0xBADC0FFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF)
        if not self.cpuid_check_result(result, expected):
            self._module_failed = True

    def _test_register_mask(self):
        result = self.cpuid(0xF00D, 0xFFFFFFFFFFFFFFFF)
        # BUG: Chipsec doesn't mask upper bits of rcx after calling CPUID
        expected = (0xBADC0FFE, 0xFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFF)
        if not self.cpuid_check_result(result, expected):
            self._module_failed = True

