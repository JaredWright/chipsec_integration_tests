"""
A chipsec module to test Bareflank CPUID ACK leaf
"""

from bareflank.base_module import *
from chipsec.hal.cpuid import *

_MODULE_NAME = 'CPUID ACK Integration Test'
TAGS = ["BAREFLANK"]

class test_ack(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

        # In/out values for CPUID instruction
        self.eax = 0x4BF00000
        self.ebx = 0x0
        self.ecx = 0x0
        self.edx = 0x0

        # Expected output values
        self.expected_eax = 0x4BF00001
        self.expected_ebx = 0x0
        self.expected_ecx = 0x0
        self.expected_edx = 0x0

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        self.load_vmm(module_argv, "bfvmm_static")

        self._print_run()
        cpuid = CpuID(self.cs)
        (self.eax, self.ebx, self.ecx, self.edx) = cpuid.cpuid(self.eax, self.ecx)

        self.unload_vmm(module_argv)

        if self.eax == self.expected_eax  \
        and self.ebx == self.expected_ebx \
        and self.ecx == self.expected_ecx \
        and self.edx == self.expected_edx:
            self._print_pass()
            return ModuleResult.PASSED
        else:
            self._print_fail()
            return ModuleResult.FAILED

    def _print_run(self):
        msg = "CPUID".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(self.eax, 8)
        msg += 'ecx=0x{0:0{1}X} '.format(self.ecx, 8)
        self.logger.log_good(msg)

    def _print_pass(self):
        msg = "Returned:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(self.eax, 8)
        msg += 'ebx=0x{0:0{1}X} '.format(self.ebx, 8)
        msg += 'ecx=0x{0:0{1}X} '.format(self.ecx, 8)
        msg += 'edx=0x{0:0{1}X} '.format(self.edx, 8)

        self.logger.log_good(msg)

    def _print_fail(self):
        msg = "Returned:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(self.eax, 8)
        msg += 'ebx=0x{0:0{1}X} '.format(self.ebx, 8)
        msg += 'ecx=0x{0:0{1}X} '.format(self.ecx, 8)
        msg += 'edx=0x{0:0{1}X} '.format(self.edx, 8)
        self.logger.log_bad(msg)

        msg = "Expected:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(self.expected_eax, 8)
        msg += 'ebx=0x{0:0{1}X} '.format(self.expected_ebx, 8)
        msg += 'ecx=0x{0:0{1}X} '.format(self.expected_ecx, 8)
        msg += 'edx=0x{0:0{1}X} '.format(self.expected_edx, 8)
        self.logger.log_bad(msg)
