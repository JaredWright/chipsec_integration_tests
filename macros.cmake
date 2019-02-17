find_package(Python2 COMPONENTS Interpreter REQUIRED)
set(PYTHONPATH ${CHIPSEC_DIR}:${CMAKE_CURRENT_LIST_DIR})
set(CHIPSEC_DIR ${USERSPACE_PREFIX_PATH}/bin/chipsec)
set(CHIPSEC_LOG_DIR ${DEPENDS_DIR}/chipsec/${USERSPACE_PREFIX}/log)
file(MAKE_DIRECTORY ${CHIPSEC_LOG_DIR})
set(BFM_EXECUTABLE ${USERSPACE_PREFIX_PATH}/bin/bfm)

message(STATUS "CHIPSEC Log DIR: ${CHIPSEC_LOG_DIR}")

function(chipsec_test NAME)
    set(options VERBOSE)
    set(singleVal VMM_EXECUTABLE CHIPSEC_MODULE)
    cmake_parse_arguments(ARG "${options}" "${singleVal}" "" ${ARGN})

    message(STATUS "Adding chipsec test: ${NAME}")

    unset(USE_LOG_FILE)
    if(ARG_VERBOSE)
        set(USE_LOG_FILE "")
    else()
        set(USE_LOG_FILE "-l ${CHIPSEC_LOG_DIR}/${NAME}.log")
    endif()

    add_custom_command(
        TARGET chipsec_test
        COMMAND
            PYTHONPATH=${PYTHONPATH}
            sudo ${Python2_EXECUTABLE}
            ${CHIPSEC_DIR}/chipsec_main.py -i -m ${ARG_CHIPSEC_MODULE}
            -a ${BFM_EXECUTABLE},${VMM_PREFIX_PATH}/bin/${ARG_VMM_EXECUTABLE}_static
            # -l ${CHIPSEC_LOG_DIR}/${NAME}.log
            # -I /path/to/my/modules
            # -t BAREFLANK # <-- specify a module "tag" to run
        USES_TERMINAL
    )

    add_custom_target(
        chipsec_test_${NAME}
        COMMAND
            PYTHONPATH=${PYTHONPATH}
            sudo ${Python2_EXECUTABLE}
            ${CHIPSEC_DIR}/chipsec_main.py -i -m ${ARG_CHIPSEC_MODULE}
            -a ${BFM_EXECUTABLE},${VMM_PREFIX_PATH}/bin/${ARG_VMM_EXECUTABLE}_static
            # -l ${CHIPSEC_LOG_DIR}/${NAME}.log
            # -I /path/to/my/modules
            # -t BAREFLANK # <-- specify a module "tag" to run
        USES_TERMINAL
    )
endfunction(chipsec_test)


