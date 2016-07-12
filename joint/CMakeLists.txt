project(joint-core)

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
	joint/JointCore.cpp
)

add_library(joint-core SHARED ${JOINT_CORE_SRC})
target_link_libraries(joint-core ${CMAKE_THREAD_LIBS_INIT})

################################################################################

set(JOINT_SRC
	joint/Joint.cpp
)

add_library(joint SHARED ${JOINT_SRC})
target_link_libraries(joint joint-core joint-python ${CMAKE_THREAD_LIBS_INIT})
