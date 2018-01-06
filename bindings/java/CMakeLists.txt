project(joint-java)

set(CMAKE_JAVA_TARGET_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

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
    org/joint/ExceptionUtils.java
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
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_definitions(-Wall)
    add_definitions(-std=c++11)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${JNI_INCLUDE_DIRS})

set(JOINT_JAVA_SOURCES
    JointJava.cpp
    JointJava.h
    binding/Binding.cpp
    binding/Binding.hpp
    binding/Boxing.hpp
    binding/JavaBindingInfo.cpp
    binding/JavaBindingInfo.hpp
    binding/JointJavaContext.cpp
    binding/JointJavaContext.hpp
    binding/JointJavaCoreContext.cpp
    binding/JointJavaCoreContext.hpp
    binding/Marshaller.hpp
    binding/Module.cpp
    binding/Module.hpp
    binding/Object.cpp
    binding/Object.hpp
    jni/ArrayJNI.cpp
    jni/InterfaceDescriptorJNI.cpp
    jni/JNIHelpers.hpp
    jni/JointExceptionJNI.cpp
    jni/JointObjectJNI.cpp
    jni/ModuleContextJNI.cpp
    utils/Env.hpp
    utils/JPtr.hpp
    utils/JniError.cpp
    utils/JniError.hpp
    utils/TryOrTerminate.hpp
    utils/Utils.cpp
    utils/Utils.hpp
)

add_library(joint-java SHARED ${JOINT_JAVA_SOURCES})
target_link_libraries(joint-java joint ${JNI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
auto_source_group(${JOINT_JAVA_SOURCES})

install(TARGETS
    joint-java
    COMPONENT java
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

auto_folder(joint-java joint_jar)
