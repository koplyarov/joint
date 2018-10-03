find_program(GO_EXECUTABLE
    NAMES "go"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(go
    FOUND_VAR GO_FOUND
    REQUIRED_VARS GO_EXECUTABLE
)


if (GO_FOUND)
    set(GO_WORKSPACE_DIR "${CMAKE_BINARY_DIR}/go_workspace")
    execute_process(
        COMMAND go env GOARCH
        OUTPUT_VARIABLE __GOARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND go env GOOS
        OUTPUT_VARIABLE __GOOS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(GO_DYNLINK_DIR "${GO_WORKSPACE_DIR}/pkg/${__GOOS}_${__GOARCH}_dynlink")

    macro(__GO_PRIVATE_REL_TO_ABS OUT_VAR F DIR)
        if(IS_ABSOLUTE ${F})
            set(${OUT_VAR} ${F})
        else()
            set(${OUT_VAR} ${DIR}/${F})
        endif()
    endmacro(__GO_PRIVATE_REL_TO_ABS)

    macro(ADD_GO_TARGET TARGET_NAME TARGET_TYPE GO_PKG_NAME)
        cmake_parse_arguments(__GO "" "" "ENV" ${ARGN})

        set(__GO_OUT "")

        if (${TARGET_TYPE} STREQUAL "EXECUTABLE")
            set(__GO_TARGET_BUILDMODE "")
            set(__GO_TARGET_ADDITIONAL_DEPENDENCIES "")
        elseif (${TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
            set(__GO_TARGET_BUILDMODE "")
            set(__GO_TARGET_ADDITIONAL_DEPENDENCIES "")
        elseif (${TARGET_TYPE} STREQUAL "SHARED_LIBRARY")
            #set(__GO_TARGET_BUILDMODE -buildmode=c-shared -linkshared)
            set(__GO_TARGET_BUILDMODE -buildmode=c-shared)
            #set(__GO_TARGET_ADDITIONAL_DEPENDENCIES go_shared_std_library)

            #if (NOT TARGET go_shared_std_library)
            #    add_custom_target(
            #        go_shared_std_library
            #        COMMAND ${CMAKE_COMMAND} -E env "GOPATH=${GO_WORKSPACE_DIR}" ${__GO_ENV} go install -buildmode=c-shared std
            #    )
            #endif()
            set(__GO_OUT -o ${GO_DYNLINK_DIR}/lib${GO_PKG_NAME}.so)
        else()
            message(FATAL_ERROR "Unknown target: ${TARGET_TYPE}")
        endif()

        foreach(__GO_SOURCE ${__GO_UNPARSED_ARGUMENTS})
            __go_private_rel_to_abs(__GO_SOURCE_ABS ${__GO_SOURCE} ${CMAKE_CURRENT_SOURCE_DIR})

            get_filename_component(__GO_SOURCE_FILENAME ${__GO_SOURCE} NAME)
            set(__GO_DST_SOURCE_ABS ${GO_WORKSPACE_DIR}/src/${GO_PKG_NAME}/${__GO_SOURCE_FILENAME})
            list(APPEND __GO_SOURCES ${__GO_DST_SOURCE_ABS})

            add_custom_command(
                OUTPUT "${__GO_DST_SOURCE_ABS}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${__GO_SOURCE_ABS}" "${__GO_DST_SOURCE_ABS}"
                DEPENDS "${__GO_SOURCE_ABS}"
            )
        endforeach()

        add_custom_target(
            ${TARGET_NAME} ALL
            COMMAND ${CMAKE_COMMAND} -E env "GOPATH=${GO_WORKSPACE_DIR}" ${__GO_ENV} go build ${__GO_TARGET_BUILDMODE} ${__GO_OUT} ${GO_PKG_NAME}
            DEPENDS ${__GO_SOURCES}
        )
    endmacro(ADD_GO_TARGET)
endif()

