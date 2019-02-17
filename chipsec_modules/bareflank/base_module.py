"""
Base Bareflank chipsec module

Provides helpers for loading/unloading VMMs, and performing common operations
"""

import subprocess
import os

from chipsec.module_common import *

_MODULE_NAME = 'Bareflank Base'

class BareflankBaseModule(BaseModule):
    def __init__(self):
        BaseModule.__init__(self)

    def is_supported(self):
        return True

    def run(self, module_argv):
        pass

    def load_vmm(self, module_argv, vmm_name):
        if len(module_argv) < 2:
            raise Exception("Failed to load vmm, no BFM or VMM paths specified")

        bfm_path = str(module_argv[0])
        vmm_dir = str(module_argv[1])
        vmm_path = os.path.join(vmm_dir, str(vmm_name))
        if subprocess.call([bfm_path, "load", vmm_path]):
            raise Exception("Failed to load vmm: " + str(vmm_path))

        if subprocess.call([bfm_path, "start"]):
            raise Exception("Failed to load vmm: " + str(vmm_path))

    def unload_vmm(self, module_argv):
        bfm_path = str(module_argv[0])
        ret = subprocess.call([bfm_path, "stop"])
        ret = subprocess.call([bfm_path, "unload"])

    # -------------------------------------------------------------------------
    # CPUID helpers
    # -------------------------------------------------------------------------
    def cpuid(self, eax, ecx):
        msg = "CPUID".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(eax, 8)
        msg += 'ecx=0x{0:0{1}X} '.format(ecx, 8)
        self.logger.log_good(msg)

        return self.cs.helper.cpuid(eax, ecx)

    def cpuid_check_result(self, cpuid_result, expected):
        if cpuid_result[0] == expected[0]  \
        and cpuid_result[1] == expected[1] \
        and cpuid_result[2] == expected[2] \
        and cpuid_result[3] == expected[3]:
            self._cpuid_print_pass(cpuid_result)
            return True
        else:
            self._cpuid_print_fail(cpuid_result, expected)
            return False

    def _cpuid_print_pass(self, cpuid_result):
        msg = "Returned:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(cpuid_result[0], 8)
        msg += 'ebx=0x{0:0{1}X} '.format(cpuid_result[1], 8)
        msg += 'ecx=0x{0:0{1}X} '.format(cpuid_result[2], 8)
        msg += 'edx=0x{0:0{1}X} '.format(cpuid_result[3], 8)

        self.logger.log_good(msg)

    def _cpuid_print_fail(self, cpuid_result, expected):
        msg = "Returned:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(cpuid_result[0], 8)
        msg += 'ebx=0x{0:0{1}X} '.format(cpuid_result[1], 8)
        msg += 'ecx=0x{0:0{1}X} '.format(cpuid_result[2], 8)
        msg += 'edx=0x{0:0{1}X} '.format(cpuid_result[3], 8)
        self.logger.log_bad(msg)

        msg = "Expected:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(expected[0], 8)
        msg += 'ebx=0x{0:0{1}X} '.format(expected[1], 8)
        msg += 'ecx=0x{0:0{1}X} '.format(expected[2], 8)
        msg += 'edx=0x{0:0{1}X} '.format(expected[3], 8)
        self.logger.log_bad(msg)
