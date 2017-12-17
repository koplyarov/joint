project(joint-native)

find_package(Threads)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    add_definitions(-Wall)
    add_definitions(-std=c++11)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
    JointNative.cpp
    binding/Binding.cpp
    binding/Module.cpp
    utils/DynamicLibrary.cpp
)

add_library(joint-native SHARED ${SOURCES})
target_link_libraries(joint-native joint ${CMAKE_THREAD_LIBS_INIT})

auto_source_group(${SOURCES})
auto_folder(joint-native)

install(TARGETS
    joint-native
    COMPONENT native
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)