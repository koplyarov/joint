project(joint)

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

################################################################################

set(JOINT_CORE_SRC
	joint/Joint.cpp
)

add_library(joint SHARED ${JOINT_CORE_SRC})
target_link_libraries(joint ${CMAKE_THREAD_LIBS_INIT})

################################################################################

set(JOINT_SRC
	joint/JointLoader.cpp
)

add_library(joint-loader SHARED ${JOINT_SRC})
target_link_libraries(joint-loader joint joint-python joint-cpp ${CMAKE_THREAD_LIBS_INIT})
