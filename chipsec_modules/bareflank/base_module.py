"""
Base Bareflank chipsec module

Provides helpers for loading/unloading VMMs, and performing common operations
"""

import subprocess
import os
import inspect

from chipsec.module_common import *

_MODULE_NAME = 'Bareflank Base'

class BareflankBaseModule(BaseModule):
    def __init__(self):
        BaseModule.__init__(self)
        class_module = inspect.getmodule(self)
        self.logger.start_test(str(class_module._MODULE_NAME))
        self.logger.log_heading(str(class_module.__doc__))

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

        self.logger.log_good("VMM Loaded:  " + str(vmm_name))

    def unload_vmm(self, module_argv):
        bfm_path = str(module_argv[0])
        ret = subprocess.call([bfm_path, "stop"])
        ret = subprocess.call([bfm_path, "unload"])
        self.logger.log_good("VMM Unloaded")

    # -------------------------------------------------------------------------
    # CPUID helpers
    # -------------------------------------------------------------------------
    def cpuid(self, eax, ecx):
        msg = "CPUID".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(eax, 8)
        msg += 'ecx=0x{0:0{1}X} '.format(ecx, 8)
        self.logger.log_good(msg)

        return self.cs.helper.cpuid(eax, ecx)

    def cpuid_print(self, cpuid_result):
        msg = "Returned:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(cpuid_result[0], 8)
        msg += 'ebx=0x{0:0{1}X} '.format(cpuid_result[1], 8)
        msg += 'ecx=0x{0:0{1}X} '.format(cpuid_result[2], 8)
        msg += 'edx=0x{0:0{1}X} '.format(cpuid_result[3], 8)

        self.logger.log_good(msg)

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

    # -------------------------------------------------------------------------
    # MSR helpers
    # -------------------------------------------------------------------------
    def rdmsr(self, address, core_id = 0):
        msg = "RDMSR".ljust(13)
        msg += 'ecx=0x{0:0{1}X} '.format(address, 8)
        msg += 'core_id=0x{0:0{1}X} '.format(core_id, 8)
        self.logger.log_good(msg)

        return self.cs.helper.read_msr(core_id, address)

    def rdmsr_check_result(self, rdmsr_result, expected):
        if rdmsr_result[0] == expected[0]  \
        and rdmsr_result[1] == expected[1]:
            self._rdmsr_print_pass(rdmsr_result)
            return True
        else:
            self._rdmsr_print_fail(rdmsr_result, expected)
            return False

    def _rdmsr_print_pass(self, rdmsr_result):
        msg = "Returned:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(rdmsr_result[0], 8)
        msg += 'edx=0x{0:0{1}X} '.format(rdmsr_result[1], 8)

        self.logger.log_good(msg)

    def _rdmsr_print_fail(self, rdmsr_result, expected):
        msg = "Returned:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(rdmsr_result[0], 8)
        msg += 'edx=0x{0:0{1}X} '.format(rdmsr_result[1], 8)
        self.logger.log_bad(msg)

        msg = "Expected:".ljust(13)
        msg += 'eax=0x{0:0{1}X} '.format(expected[0], 8)
        msg += 'edx=0x{0:0{1}X} '.format(expected[1], 8)
        self.logger.log_bad(msg)
