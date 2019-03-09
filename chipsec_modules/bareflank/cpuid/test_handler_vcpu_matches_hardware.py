"""
Test Scenario:

Make sure that the vcpu passed into a cpuid handler already contains the
values reported by hardware for the cpuid instruction that caused the vmexit.
A VMM is loaded that registers a single cpuid handler for an existing cpuid
leaf, and behaves as follows:
  - Save the vcpu state associated with cpuid instruction "outputs"
    (i.e. eax, ebx, ecx, edx).
  - Re-run CPUID manually and re-populate the vcpu with hardware state.
    This should result in the vcpu's registers retaining the same values
  - Return true to end the handler chain

The "original" vcpu state (at the handler's entry point) is exposed through a
sperate cpuid emulator for comparison
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID Handler Matches Hardware Test'
TAGS = ["BAREFLANK"]

class test_handler_vcpu_matches_hardware(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
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
