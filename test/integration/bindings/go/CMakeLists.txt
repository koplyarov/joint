project(joint-go-test)

find_package(Go REQUIRED)

joint_generate_adapters(go ${CMAKE_CURRENT_BINARY_DIR}/adapters.go
    ${IDL_DIR}/IBasicTests.idl
    ${IDL_DIR}/ICastTests.idl
    ${IDL_DIR}/IEnumTests.idl
    ${IDL_DIR}/IObjectTests.idl
    ${IDL_DIR}/IStarterTests.idl
)

set(SOURCES
    joint_go_tests/component.go
    ${CMAKE_CURRENT_BINARY_DIR}/adapters.go
    ENV "CGO_CFLAGS=-I${CMAKE_SOURCE_DIR}/core"
)

add_go_target(
    joint_go_txsts SHARED_LIBRARY joint_go_txsts
    ${SOURCES}
)

joint_make_module_package(tests-component-go
    PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/go/Tests
    MANIFEST Tests.jm
    DEPENDS joint_go_txsts
)

joint_add_module_content(tests-component-go
    DIRECTORY .
    FILES ${GO_DYNLINK_DIR}/libjoint_go_txsts.so
)
