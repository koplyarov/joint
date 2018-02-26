project(joint-gen)

find_package(Mypy)
find_package(Pylint)
find_package(Pep8)
find_package(PymoduleChecker)

if (MYPY_FOUND)
    message(STATUS "mypy version: ${MYPY_VERSION}")

    set(MYPY_LOAD_PLUGINS "")

    add_custom_target(joint-gen-mypy ALL
        COMMAND ${MYPY_EXECUTABLE}
            --py2
            ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running mypy on joint-gen sources"
    )
else()
    message(WARNING "pylint not found!")
endif()

if (PYLINT_FOUND)
    message(STATUS "pylint version: ${PYLINT_VERSION}")

    set(PYLINT_LOAD_PLUGINS "")

    CheckPymodule("pylint_quotes" PYLINT_QUOTES_FOUND)
    if (${PYLINT_QUOTES_FOUND})
        set(PYLINT_LOAD_PLUGINS ${PYLINT_LOAD_PLUGINS} --load-plugins pylint_quotes)
    endif()

    add_custom_target(joint-gen-pylint ALL
        COMMAND ${PYLINT_EXECUTABLE}
            ${PYLINT_LOAD_PLUGINS}
            --reports=n
            --msg-template=\"{path}:{line}: {msg_id} {symbol}, {obj} {msg}\"
            --rcfile=${CMAKE_CURRENT_SOURCE_DIR}/../etc/pylintrc
            ${CMAKE_CURRENT_SOURCE_DIR}/joint_gen.py
        COMMENT "Running pylint on joint-gen sources"
    )
else()
    message(WARNING "pylint not found!")
endif()

if (PEP8_FOUND)
    message(STATUS "pep8 version: ${PEP8_VERSION}")
    add_custom_target(joint-gen-pep8 ALL
        COMMAND ${PEP8_EXECUTABLE}
            --max-line-length=200
            ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running pep8 on joint-gen sources"
    )
else()
    message(WARNING "pep8 not found!")
endif()

################################################################################

set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)

install(FILES
    setup.py
    joint_gen.py
    DESTINATION share/joint-gen
    COMPONENT gen)

install(DIRECTORY
    joint
    DESTINATION share/joint-gen
    COMPONENT gen
    FILES_MATCHING PATTERN "*.py")

install(DIRECTORY
    idl
    DESTINATION share/joint
    COMPONENT gen
    FILES_MATCHING PATTERN "*.idl")

set(CPACK_DEBIAN_GEN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/deb/postinst;${CMAKE_CURRENT_SOURCE_DIR}/deb/prerm" PARENT_SCOPE)
set(CPACK_RPM_gen_POST_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/rpm/postinst" PARENT_SCOPE)
set(CPACK_RPM_gen_PRE_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/rpm/prerm" PARENT_SCOPE)
