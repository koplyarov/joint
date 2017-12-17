project(joint-cpp)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    add_definitions(-Wall)
    add_definitions(-std=c++11)
endif()

set(SOURCES
    joint.cpp/Array.hpp
    joint.cpp/BytesView.hpp
    joint.cpp/Component.hpp
    joint.cpp/detail/AccessorHolder.hpp
    joint.cpp/detail/ClassContentChecks.hpp
    joint.cpp/detail/Config.hpp
    joint.cpp/detail/Dummy.hpp
    joint.cpp/detail/GeneratedCodeEpilogue.hpp
    joint.cpp/detail/GeneratedCodePrologue.hpp
    joint.cpp/detail/Iterator.hpp
    joint.cpp/detail/JointCall.hpp
    joint.cpp/detail/NonCopyable.hpp
    joint.cpp/detail/RetValueGuard.hpp
    joint.cpp/Exception.hpp
    joint.cpp/MetaProgramming.hpp
    joint.cpp/Module.hpp
    joint.cpp/Ptr.hpp
    joint.cpp/Result.hpp
    joint.cpp/String.hpp
    joint.cpp/TypeList.hpp
    test/LinearizedInterfacesList.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(joint-cpp ${SOURCES})
target_link_libraries(joint-cpp)
auto_source_group(${SOURCES})
auto_folder(joint-cpp)

# TODO: install headers