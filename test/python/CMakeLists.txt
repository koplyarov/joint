project(test-python)

joint_generate_adapters(python ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py
	../idl/IBasicTests.idl
	../idl/IObjectTests.idl
	../idl/IEnumTests.idl
	../idl/ILifetimeTests.idl
	../idl/ICastTests.idl
	../idl/IExceptionTests.idl
	../idl/IInterfaceChecksumTests.idl
	../idl/IStructTests.idl
	../idl/IArrayTests.idl
)

add_custom_target(test-adapters-py ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py)

joint_make_module_package(tests-component-py
	PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/python/Tests
	MANIFEST Tests.jm
	DEPENDS test-adapters-py
)

joint_add_module_content(tests-component-py
	DIRECTORY Package
	FILES __init__.py Component.py ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py
)
