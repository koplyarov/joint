project(joint-go)

find_package(Go REQUIRED)

set(SOURCES
    joint/accessor.go
    joint/objects_map.go
)

add_go_target(
    joint-go STATIC_LIBRARY joint
    ${SOURCES}
    ENV "CGO_CFLAGS=-I${CMAKE_SOURCE_DIR}/core"
)
