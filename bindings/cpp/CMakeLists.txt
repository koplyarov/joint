project(joint-cpp)

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(JOINT_CPP_CORE_SRC
	JointCppCore.cpp
	binding/Binding.cpp
	utils/DynamicLibrary.cpp
)

add_library(joint-cpp SHARED ${JOINT_CPP_CORE_SRC})
target_link_libraries(joint-cpp joint ${CMAKE_THREAD_LIBS_INIT})
