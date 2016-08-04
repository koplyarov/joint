project(joint-gen)

set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)

install(FILES
	setup.py
	joint-gen.py
	DESTINATION share/joint-gen
	COMPONENT gen)

install(DIRECTORY
	joint
	DESTINATION share/joint-gen
	COMPONENT gen)

set(CPACK_DEBIAN_GEN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/deb/postinst;${CMAKE_CURRENT_SOURCE_DIR}/deb/prerm" PARENT_SCOPE)
