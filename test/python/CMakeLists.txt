project(test-c)

include(${CMAKE_SOURCE_DIR}/joint-gen/joint.cmake)

joint_generate_adapters(python ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py
	../idl/IBasicTests.idl
	../idl/IObjectTests.idl
	../idl/ILifetimeTests.idl)

add_custom_target(test-adapters-py ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py)

