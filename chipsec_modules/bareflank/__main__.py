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

cs.load_module("bareflank/cpuid/test_ack.py", argv)
cs.load_module("bareflank/cpuid/test_emulation.py", argv)
cs.load_module("bareflank/cpuid/test_pass_through.py", argv)
cs.load_module("bareflank/cpuid/test_repeat_vmm_init.py", argv)

# Run
cs.run_loaded_modules()
