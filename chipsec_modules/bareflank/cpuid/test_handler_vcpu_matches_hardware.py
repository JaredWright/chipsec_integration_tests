"""
A chipsec module to test if Bareflank CPUID handlers receive a vcpu that is
already populated with hardware "output" values
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Handler Matches Hardware Test'
TAGS = ["BAREFLANK"]

class test_handler_vcpu_matches_hardware(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)
        test_passed = True

        # Real CPUID leaf (no vmm loaded yet)
        real_cpuid_result = self.cpuid(0x0, 0x0)

        self.load_vmm(module_argv, "integration_cpuid_handler_vcpu_matches_hardware_static")

        # Same CPUID leaf with a vmm loaded
        handled_cpuid_result = self.cpuid(0x0, 0x0)

        # Emulated CPUID leaf that should contain the same values
        emulated_cpuid_result = self.cpuid(0xF00D, 0x0)

        self.unload_vmm(module_argv)

        # ALL THREE results should match
        if not self.cpuid_check_result(real_cpuid_result, handled_cpuid_result):
            self.logger.log_failed("Real CPUID leaf does not match handled CPUID leaf")
            test_passed = False;

        if not self.cpuid_check_result(handled_cpuid_result, emulated_cpuid_result):
            self.logger.log_failed("Handled CPUID leaf does not match emulated CPUID leaf")
            test_passed = False;

        if test_passed == True:
            self.logger.log_passed(_MODULE_NAME)
            return ModuleResult.PASSED
        else:
            self.logger.log_failed(_MODULE_NAME)
            return ModuleResult.FAILED
