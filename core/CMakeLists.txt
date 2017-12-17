project(joint)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    set(PLATFORM_LIBS "")
    add_definitions(-DYY_NO_UNISTD_H)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D__STDC_VERSION__=199901L)
else()
    add_definitions(-Wall)
    add_definitions(-std=c++11)
    set(PLATFORM_LIBS dl)
    set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.cpp" PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-unneeded-internal-declaration -Wno-error")
endif()

################################################################################

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
if (FLEX_FOUND AND BISON_FOUND)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.y.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/joint/private/JsonParser.y
        COMMAND ${BISON_EXECUTABLE}
        ARGS --defines=${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.y.hpp -o ${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.y.cpp ${CMAKE_CURRENT_SOURCE_DIR}/joint/private/JsonParser.y
        COMMENT "Generating JsonParser.y.cpp"
    )

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/joint)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/joint/private/JsonParser.l
        COMMAND ${FLEX_EXECUTABLE}
        ARGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.hpp -o${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.cpp ${CMAKE_CURRENT_SOURCE_DIR}/joint/private/JsonParser.l
        COMMENT "Generating JsonParser.l.cpp"
    )
endif()

set(SOURCES
    joint/Joint.h
    joint/JsonParser.l
    joint/JsonParser.y
    joint/private/Joint_Array.cpp
    joint/private/Joint_Array.hpp
    joint/private/JsonNode.cpp
    joint/private/JsonNode.hpp
    joint/private/JsonObjectAssembler.cpp
    joint/private/JsonObjectAssembler.hpp
    joint/private/JsonParser.cpp
    joint/private/JsonParser.hpp
    joint/public/AccessorUtils.h
    joint/public/Array.cpp
    joint/public/Array.h
    joint/public/Assert.cpp
    joint/public/Assert.h
    joint/public/BasicTypes.h
    joint/public/BindingAccessor.h
    joint/public/BindingAccessorVTable.h
    joint/public/Error.cpp
    joint/public/Error.h
    joint/public/Exception.cpp
    joint/public/Exception.h
    joint/public/JointConfig.h
    joint/public/JointCoreApi.h
    joint/public/Log.cpp
    joint/public/Log.h
    joint/public/Manifest.cpp
    joint/public/Manifest.h
    joint/public/ModuleAccessor.h
    joint/public/ModuleAccessorVTable.h
    joint/public/ObjectAccessor.h
    joint/public/ObjectAccessorVTable.h
    joint/public/Type.h
    joint/public/Value.h
)

set(GENERATED_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.l.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/joint/JsonParser.y.cpp
)


add_library(joint SHARED ${SOURCES} ${GENERATED_SOURCES})
target_link_libraries(joint ${CMAKE_THREAD_LIBS_INIT})

source_group(generated FILES ${GENERATED_SOURCES})
auto_source_group(${SOURCES})
auto_folder(joint)


################################################################################

set(JOINT_LOADER_SOURCES
    joint/JointLoader.h
    joint/JointLoader.cpp
)

add_library(joint-loader SHARED ${JOINT_LOADER_SOURCES})
target_link_libraries(joint-loader joint joint-python joint-native joint-java ${CMAKE_THREAD_LIBS_INIT} ${PLATFORM_LIBS})
auto_source_group(${JOINT_LOADER_SOURCES})
auto_folder(joint-loader)

################################################################################

install(FILES
    joint/Joint.h
    joint/JointConfig.h
    joint/JointLoader.h
    DESTINATION include
    COMPONENT dev
)

install(TARGETS
    joint
    joint-loader
    COMPONENT runtime
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
