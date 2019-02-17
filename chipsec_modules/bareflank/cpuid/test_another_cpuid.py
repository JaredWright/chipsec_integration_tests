"""
A chipsec module that should fail
"""

from bareflank.base_module import *

_MODULE_NAME = 'This test should fail'

class test_another_cpuid(BareflankBaseModule):
    def __init__(self):
        BareflankBaseModule.__init__(self)

    def run(self, module_argv):
        self.logger.start_test(_MODULE_NAME)

        self.logger.log_failed("This should fail")
        return ModuleResult.FAILED
