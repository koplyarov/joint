project(test-c)

joint_generate_adapters(python ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py
	../idl/IBasicTests.idl
	../idl/IObjectTests.idl
	../idl/IEnumTests.idl
	../idl/ILifetimeTests.idl
	../idl/ICastTests.idl
	../idl/IExceptionTests.idl
	../idl/IInterfaceChecksumTests.idl
	../idl/IStructTests.idl
)

add_custom_target(test-adapters-py ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py)

