find_program(MYPY_EXECUTABLE mypy PATHS /usr/bin)

if (MYPY_EXECUTABLE)
    execute_process(COMMAND ${MYPY_EXECUTABLE} --version OUTPUT_VARIABLE MYPY_VERSION_RAW ERROR_QUIET)
    if (MYPY_VERSION_RAW)
        string(REGEX REPLACE "^mypy ([0-9]+.[0-9]+).*" "\\1" MYPY_VERSION ${MYPY_VERSION_RAW})
    else ()
        set(MYPY_VERSION "unknown")
    endif()
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Mypy DEFAULT_MSG MYPY_EXECUTABLE)

mark_as_advanced(MYPY_EXECUTABLE MYPY_VERSION)
