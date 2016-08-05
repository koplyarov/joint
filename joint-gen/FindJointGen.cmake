find_program(JOINT_GEN_SCRIPT
	NAMES "joint-gen.py"
    PATHS 
    ${CMAKE_CURRENT_LIST_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(joint_gen
	FOUND_VAR JOINT_GEN_FOUND
    REQUIRED_VARS JOINT_GEN_SCRIPT
)


if (JOINT_GEN_FOUND)
    set(__JOINT_PRIVATE_IMPORT_DIRECTORIES "")
    set(__JOINT_PRIVATE_GEN_BINARY ${JOINT_GEN_SCRIPT})

    macro(JOINT_IMPORT_DIRECTORIES)
        foreach(DIR ${ARGN})
            if(IS_ABSOLUTE ${DIR})
                list(APPEND __JOINT_PRIVATE_IMPORT_DIRECTORIES ${DIR})
            else()
                list(APPEND __JOINT_PRIVATE_IMPORT_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/${DIR})
            endif()
        endforeach()
    endmacro(JOINT_IMPORT_DIRECTORIES)

    macro(JOINT_GENERATE_ADAPTERS LANGUAGE OUTPUT)
        set(__JOINT_CMD ${__JOINT_PRIVATE_GEN_BINARY} -l ${LANGUAGE} -o ${OUTPUT})

        foreach(DIR ${__JOINT_PRIVATE_IMPORT_DIRECTORIES})
            set(__JOINT_CMD ${__JOINT_CMD} -I ${DIR})
        endforeach()

        foreach(IDL ${ARGN})
            if(IS_ABSOLUTE ${IDL})
                set(__JOINT_CMD ${__JOINT_CMD} ${IDL})
            else()
                set(__JOINT_CMD ${__JOINT_CMD} ${CMAKE_CURRENT_SOURCE_DIR}/${IDL})
            endif()
        endforeach()

        add_custom_command(
            OUTPUT ${OUTPUT}
            COMMAND ${__JOINT_CMD}
            DEPENDS ${ARGN}
        )

        set_source_files_properties(${OUTPUT} PROPERTIES GENERATED 1)
    endmacro(JOINT_GENERATE_ADAPTERS)
endif()
