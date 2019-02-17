"""
Base Bareflank chipsec module

Provides helpers for loading/unloading VMMs, etc
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
