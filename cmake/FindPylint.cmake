find_program(PYLINT_EXECUTABLE pylint PATHS /usr/bin)

if (PYLINT_EXECUTABLE)
    execute_process(COMMAND ${PYLINT_EXECUTABLE} --version OUTPUT_VARIABLE PYLINT_VERSION_RAW ERROR_QUIET)
    if (PYLINT_VERSION_RAW)
        string(REGEX REPLACE "^pylint ([0-9]+.[0-9]+.[0-9]+),.*" "\\1" PYLINT_VERSION ${PYLINT_VERSION_RAW})
    else ()
        set(PYLINT_VERSION "unknown")
    endif()
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pylint DEFAULT_MSG PYLINT_EXECUTABLE)

mark_as_advanced(PYLINT_EXECUTABLE PYLINT_VERSION)
