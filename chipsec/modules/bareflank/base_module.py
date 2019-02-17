"""
Base Bareflank chipsec module

Provides helpers for loading/unloading VMMs, etc
"""

import subprocess

from chipsec.module_common import *

_MODULE_NAME = 'Bareflank Base'

class BareflankBaseModule(BaseModule):
    def __init__(self):
        BaseModule.__init__(self)

    def is_supported(self):
        return True

    def run(self, module_argv):
        pass

    def load_vmm(self, module_argv):
        bfm_path = str(module_argv[0])
        vmm_path = str(module_argv[1])
        ret = subprocess.call([bfm_path, "load", vmm_path])
        ret = subprocess.call([bfm_path, "start"])

    def unload_vmm(self, module_argv):
        bfm_path = str(module_argv[0])
        ret = subprocess.call([bfm_path, "stop"])
        ret = subprocess.call([bfm_path, "unload"])
