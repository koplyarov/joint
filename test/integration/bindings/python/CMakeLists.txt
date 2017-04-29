project(test-python)

joint_generate_adapters(python Tests_adapters.py
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

add_custom_target(test-adapters-py ALL DEPENDS Tests_adapters.py)

joint_make_module_package(tests-component-py
	PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/python/Tests
	MANIFEST Tests.jm
	DEPENDS test-adapters-py
)

joint_add_module_content(tests-component-py
	DIRECTORY Package
	FILES __init__.py Component.py ${CMAKE_CURRENT_BINARY_DIR}/Tests_adapters.py
)
