add_custom_target_category("Chipsec-Based Integration Tests")

add_custom_target(chipsec_test)

add_custom_target_info(
    TARGET chipsec_test
    COMMENT "Run chipsec integration tests"
)

find_package(Python2 COMPONENTS Interpreter REQUIRED)
set(CHIPSEC_DIR ${USERSPACE_PREFIX_PATH}/bin/chipsec)
set(CHIPSEC_MODULE_INSTALL_DIR ${USERSPACE_PREFIX_PATH}/bin/chipsec_modules)

set(PYTHONPATH ${CHIPSEC_DIR})
set(PYTHONPATH ${PYTHONPATH}:${CHIPSEC_MODULE_INSTALL_DIR})
# set(PYTHONPATH ${PYTHONPATH}:${CHIPSEC_MODULE_INSTALL_DIR}/bareflank)

set(BFM_EXECUTABLE ${USERSPACE_PREFIX_PATH}/bin/bfm)

add_custom_command(
    TARGET chipsec_test
    COMMAND
        sudo PYTHONPATH=${PYTHONPATH}
        ${Python2_EXECUTABLE} -m bareflank ${BFM_EXECUTABLE} ${VMM_PREFIX_PATH}/bin
    USES_TERMINAL
)
