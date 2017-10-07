project(joint)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	set(PLATFORM_LIBS "")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
	set(PLATFORM_LIBS dl)
	set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.cpp" PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-unneeded-internal-declaration -Wno-error")
endif()

################################################################################

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
if (FLEX_FOUND AND BISON_FOUND)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.y.cpp
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/joint/private/JsonParser.y
		COMMAND ${BISON_EXECUTABLE}
		ARGS --defines=${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.y.hpp -o ${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.y.cpp ${CMAKE_CURRENT_SOURCE_DIR}/joint/private/JsonParser.y
		COMMENT "Generating JsonParser.y.cpp"
	)

	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/joint)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.cpp
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/joint/private/JsonParser.l
		COMMAND ${FLEX_EXECUTABLE}
		ARGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.hpp -o${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.cpp ${CMAKE_CURRENT_SOURCE_DIR}/joint/private/JsonParser.l
		COMMENT "Generating JsonParser.l.cpp"
	)
endif()

set(JOINT_CORE_SRC
	joint/private/Joint_Array.cpp
	joint/private/JsonNode.cpp
	joint/private/JsonObjectAssembler.cpp
	joint/private/JsonParser.cpp
	joint/public/Array.cpp
	joint/public/Assert.cpp
	joint/public/Error.cpp
	joint/public/Exception.cpp
	joint/public/Log.cpp
	joint/public/Manifest.cpp
	${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.cpp
	${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.y.cpp
)

add_library(joint SHARED ${JOINT_CORE_SRC})
target_link_libraries(joint ${CMAKE_THREAD_LIBS_INIT})

################################################################################

set(JOINT_SRC
	joint/JointLoader.cpp
)

add_library(joint-loader SHARED ${JOINT_SRC})
target_link_libraries(joint-loader joint joint-python joint-native joint-java ${CMAKE_THREAD_LIBS_INIT} ${PLATFORM_LIBS})

################################################################################

install(FILES
	joint/Joint.h
	joint/JointConfig.h
	joint/JointLoader.h
	DESTINATION include
	COMPONENT dev)

install(TARGETS
	joint
	joint-loader
	COMPONENT runtime
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)
