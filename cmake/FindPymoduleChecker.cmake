find_package(PythonInterp REQUIRED)

if (PYTHONINTERP_FOUND)
    function(CheckPymodule MODULE RESULT_VAR)
        execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import ${MODULE}" RESULT_VARIABLE RES ERROR_QUIET)
        if(${RES} EQUAL 0)
            message(STATUS "${MODULE} python module found")
            set(${RESULT_VAR} TRUE PARENT_SCOPE)
        else()
            message(STATUS "${MODULE} python module not found")
            set(${RESULT_VAR} FALSE PARENT_SCOPE)
        endif()
    endfunction()
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pylint DEFAULT_MSG PYLINT_EXECUTABLE)

mark_as_advanced(PYLINT_EXECUTABLE PYLINT_VERSION)
