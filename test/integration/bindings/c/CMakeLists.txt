project(test-c)

include_directories(${JOINT_C_INCLUDE_DIRECTORIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (NOT MSVC)
    add_definitions(-Wall)
endif()

################################################################################

joint_generate_adapters(c Tests_adapters.h
    ${IDL_DIR}/IBasicTests.idl
    ${IDL_DIR}/IEnumTests.idl
    ${IDL_DIR}/IObjectTests.idl
    ${IDL_DIR}/ILifetimeTests.idl
    ${IDL_DIR}/ICastTests.idl
    ${IDL_DIR}/IExceptionTests.idl
    ${IDL_DIR}/IInterfaceChecksumTests.idl
    ${IDL_DIR}/IStructTests.idl
    ${IDL_DIR}/IArrayTests.idl
)

add_library(tests-c SHARED Tests.c Tests_adapters.h)
target_link_libraries(tests-c ${JOINT_LIBRARIES})

joint_make_module_package(tests-component-c
    PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/c/Tests
    MANIFEST Tests.jm
)

joint_add_module_content(tests-component-c
    DIRECTORY .
    LIBRARIES tests-c
)

auto_folder(tests-c tests-component-c)
