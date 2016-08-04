project(joint-cpp)

find_package(Threads)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(joint-cpp SHARED 
	JointCppCore.cpp
	binding/Binding.cpp
	utils/DynamicLibrary.cpp
)
target_link_libraries(joint-cpp joint ${CMAKE_THREAD_LIBS_INIT})
