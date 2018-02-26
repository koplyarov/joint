add_definitions(-D_GLIBCXX_DEBUG)

set(IDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/idl)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(JOINT_MODULE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(Joint)

add_subdirectory(bindings)

find_package(Threads)

include_directories(${JOINT_CPP_INCLUDE_DIRECTORIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    add_definitions(-Wall)
    add_definitions(-std=c++11)
endif()

################################################################################

joint_generate_adapters(c++ Tests_adapters.hpp
    idl/IStarterTests.idl
    idl/IBasicTests.idl
    idl/IObjectTests.idl
    idl/ILifetimeTests.idl
    idl/ICastTests.idl
    idl/IExceptionTests.idl
    idl/IEnumTests.idl
    idl/IStructTests.idl
    IInterfaceChecksumTests.idl
    idl/IArrayTests.idl
)

set(SOURCES
    tests/ArrayTests.hpp
    tests/BasicTests.hpp
    tests/CastTests.hpp
    tests/EnumTests.hpp
    tests/ExceptionTests.hpp
    tests/InterfaceChecksumTests.hpp
    tests/LifetimeTests.hpp
    tests/ObjectTests.hpp
    tests/StarterTests.hpp
    tests/StructTests.hpp
    tests/TestCtx.hpp
    test.cpp
)

set(GENERATED_SOURCES
    Tests_adapters.hpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(joint-test ${SOURCES} ${GENERATED_SOURCES})
target_link_libraries(joint-test test-core ${JOINT_LIBRARIES})
source_group(generated FILES ${GENERATED_SOURCES})
auto_source_group(${SOURCES})
auto_folder(joint-test)

set_target_properties(test-core PROPERTIES FOLDER test/core)
