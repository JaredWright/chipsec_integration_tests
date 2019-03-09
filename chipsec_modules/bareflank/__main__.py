import sys
import os

from chipsec_main import *

argv = sys.argv[1:]
this_dir = os.path.abspath(os.path.dirname(__file__))

# Initilize Chipsec
cs = ChipsecMain(sys.argv[1:])
cs.IMPORT_PATHS.append(this_dir)

try:
    cs._cs.init( cs._platform, cs._pch, (not cs._no_driver), cs._driver_exists )
except chipset.UnknownChipsetError , msg:
    logger().warn( "Platform is not supported (%s)." % str(msg) )
    logger().warn("Platform dependent functionality is likely to be incorrect")
except oshelper.OsHelperError as os_helper_error:
    logger().error(str(os_helper_error))
except BaseException, be:
    logger().log_bad(traceback.format_exc())

# TODO: Figure out a way to load all modules automatically
# Doesn't work: Import all python files that start with test_ as chipsec modules
#  for root, dirnames, filenames in os.walk(this_dir):
#      for filename in fnmatch.filter(filenames, 'test_*.py'):
#          cs.load_module(os.path.join(root, filename), argv)

# CPUID Tests
cs.load_module("bareflank/cpuid/test_ack.py", argv)
cs.load_module("bareflank/cpuid/test_repeat_vmm_init.py", argv)
cs.load_module("bareflank/cpuid/test_emulator_and_handler.py", argv)
cs.load_module("bareflank/cpuid/test_emulator_basic.py", argv)
cs.load_module("bareflank/cpuid/test_emulator_does_not_leak_hardware.py", argv)
cs.load_module("bareflank/cpuid/test_emulator_multiple.py", argv)
cs.load_module("bareflank/cpuid/test_emulator_inputs_preserved.py", argv)
cs.load_module("bareflank/cpuid/test_handler_basic.py", argv)
cs.load_module("bareflank/cpuid/test_handler_inputs_preserved.py", argv)
cs.load_module("bareflank/cpuid/test_handler_multiple.py", argv)
cs.load_module("bareflank/cpuid/test_handler_vcpu_matches_hardware.py", argv)
# TODO: The following test intentionally tries to cause an unhandled exception.
# Figure out how to recover from an unhandled vmexit to run this test safely
#  cs.load_module("bareflank/cpuid/test_emulator_and_handler_exception.py", argv)

#  MSR Tests
#  cs.load_module("bareflank/msr/test_rdmsr_emulate.py", argv)

# Run
cs.run_loaded_modules()
