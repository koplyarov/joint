project(joint-java)

find_package(Threads)
find_package(JNI REQUIRED)
find_package(Java REQUIRED)
include(UseJava)

set(JAVA_SOURCES
	org/joint/Accessor.java
	org/joint/AccessorBase.java
	org/joint/AccessorsContainer.java
	org/joint/Array.java
	org/joint/BuiltinTypes.java
	org/joint/Boxing.java
	org/joint/Component.java
	org/joint/ComponentProxy.java
	org/joint/InterfaceDescriptor.java
	org/joint/InterfaceId.java
	org/joint/JointException.java
	org/joint/JointObject.java
	org/joint/MethodDescriptor.java
	org/joint/ModuleContext.java
	org/joint/TypeDescriptor.java
)

set(CMAKE_JNI_TARGET TRUE)
add_jar(joint_jar
	SOURCES ${JAVA_SOURCES}
	OUTPUT_NAME joint
    OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

get_target_property(JOINT_JAR_CLASSDIR joint_jar CLASSDIR)

add_custom_command(
	OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/JointJNI.h"
	DEPENDS joint_jar ${JAVA_SOURCES}
	COMMAND "${Java_JAVAH_EXECUTABLE}"
	-jni
	-o "${CMAKE_CURRENT_BINARY_DIR}/JointJNI.h"
	-classpath "${JOINT_JAR_CLASSDIR}"
	org.joint.Array
	org.joint.InterfaceDescriptor
	org.joint.JointException
	org.joint.JointObject
	org.joint.ModuleContext
)

set_source_files_properties(
	jni/ArrayJNI.cpp
	jni/InterfaceDescriptorJNI.cpp
	jni/JointExceptionJNI.cpp
	jni/JointObjectJNI.cpp
	jni/ModuleContextJNI.cpp
	OBJECT_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/JointJNI.h"
)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${JNI_INCLUDE_DIRS})

add_library(joint-java SHARED 
	JointJava.cpp
	binding/Binding.cpp
	binding/JavaBindingInfo.cpp
	binding/JointJavaContext.cpp
	binding/Module.cpp
	binding/Object.cpp
	jni/ArrayJNI.cpp
	jni/InterfaceDescriptorJNI.cpp
	jni/JointExceptionJNI.cpp
	jni/JointObjectJNI.cpp
	jni/ModuleContextJNI.cpp
	utils/JniError.cpp
	utils/Utils.cpp
)
target_link_libraries(joint-java joint ${JNI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS
	joint-java
	COMPONENT java
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)
