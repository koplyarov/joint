project(joint)

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	set(PLATFORM_LIBS "")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
	set(PLATFORM_LIBS dl)
endif()

################################################################################

set(JOINT_CORE_SRC
	joint/Joint.cpp
	joint/private/Joint_Array.cpp
)

add_library(joint SHARED ${JOINT_CORE_SRC})
target_link_libraries(joint ${CMAKE_THREAD_LIBS_INIT})

################################################################################

set(JOINT_SRC
	joint/JointLoader.cpp
)

add_library(joint-loader SHARED ${JOINT_SRC})
target_link_libraries(joint-loader joint joint-python joint-cpp joint-java ${CMAKE_THREAD_LIBS_INIT} ${PLATFORM_LIBS})

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
