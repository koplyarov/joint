project(joint-java)

find_package(Threads)
find_package(JNI REQUIRED)
find_package(Java REQUIRED)
include(UseJava)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${JNI_INCLUDE_DIRS})

add_library(joint-java SHARED 
	JointJava.cpp
	binding/Binding.cpp
	binding/Module.cpp
	binding/Object.cpp
	utils/JniError.cpp
)
target_link_libraries(joint-java joint ${JNI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_jar(joint_jar
	SOURCES org/joint/Accessor.java
	SOURCES org/joint/AccessorsContainer.java
	SOURCES org/joint/Component.java
	SOURCES org/joint/InterfaceId.java
	OUTPUT_NAME joint
    OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

install(TARGETS
	joint-java
	COMPONENT java
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)
