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
	if (JOINT_COVERAGE)
		set(__JOINT_PRIVATE_GEN_BINARY COVERAGE_FILE=${CMAKE_SOURCE_DIR}/.coverage coverage run --rcfile ${CMAKE_SOURCE_DIR}/.coveragerc -a ${JOINT_GEN_SCRIPT})
	else()
		set(__JOINT_PRIVATE_GEN_BINARY ${JOINT_GEN_SCRIPT})
	endif()

	macro(__JOINT_PRIVATE_REL_TO_ABS OUT_VAR F DIR)
		if(IS_ABSOLUTE ${F})
			set(${OUT_VAR} ${F})
		else()
			set(${OUT_VAR} ${DIR}/${F})
		endif()
	endmacro(__JOINT_PRIVATE_REL_TO_ABS)


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
			__joint_private_rel_to_abs(IDL ${IDL} ${CMAKE_CURRENT_SOURCE_DIR})
			set(__JOINT_CMD ${__JOINT_CMD} ${IDL})
		endforeach()

		add_custom_command(
			OUTPUT ${OUTPUT}
			COMMAND ${__JOINT_CMD}
			DEPENDS ${ARGN}
		)

		set_source_files_properties(${OUTPUT} PROPERTIES GENERATED 1)
	endmacro(JOINT_GENERATE_ADAPTERS)


	macro(JOINT_MAKE_MODULE_PACKAGE TARGET_NAME)
		cmake_parse_arguments(_JOINT_MMP "" "PACKAGE_DIR;MANIFEST" "DEPENDS" ${ARGN})

		__joint_private_rel_to_abs(_JOINT_MMP_MANIFEST_ABS ${_JOINT_MMP_MANIFEST} ${CMAKE_CURRENT_SOURCE_DIR})
		__joint_private_rel_to_abs(_JOINT_${TARGET_NAME}_PACKAGE_DIR ${_JOINT_MMP_PACKAGE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

		get_filename_component(_JOINT_MMP_MANIFEST_BASENAME ${_JOINT_MMP_MANIFEST} NAME)

		add_custom_target(${TARGET_NAME} ALL)
		add_custom_command(
			TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_JOINT_MMP_MANIFEST_ABS} ${_JOINT_MMP_PACKAGE_DIR}/${_JOINT_MMP_MANIFEST}
			DEPENDS ${_JOINT_MMP_MANIFEST}
		)

		foreach(D ${_JOINT_MMP_DEPENDS})
			add_dependencies(${TARGET_NAME} ${D})
		endforeach()
	endmacro(JOINT_MAKE_MODULE_PACKAGE)


	macro(JOINT_ADD_MODULE_CONTENT TARGET_NAME)
		cmake_parse_arguments(_JOINT_MMP "" "DIRECTORY" "FILES;LIBRARIES" ${ARGN})

		if (IS_ABSOLUTE ${_JOINT_MMP_DIRECTORY})
			message(FATAL_ERROR "The directory for the joint_add_module_content command is a path inside the package, so it must not be absolute.")
		endif()

		set(_DST_DIR ${_JOINT_${TARGET_NAME}_PACKAGE_DIR}/${_JOINT_MMP_DIRECTORY})

		add_custom_command(
			TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory ${_DST_DIR}
		)

		foreach(F ${_JOINT_MMP_FILES})
			__joint_private_rel_to_abs(F_ABS ${F} ${CMAKE_CURRENT_SOURCE_DIR})

			get_filename_component(F_BASENAME ${F} NAME)

			add_custom_command(
				TARGET ${TARGET_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different ${F_ABS} ${_DST_DIR}/${F_BASENAME}
				DEPENDS ${F}
			)
		endforeach()

		foreach(L ${_JOINT_MMP_LIBRARIES})
			add_custom_command(
				TARGET ${TARGET_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${L}> ${_DST_DIR}/$<TARGET_FILE_NAME:${L}>
				DEPENDS ${L}
			)
		endforeach()
	endmacro(JOINT_ADD_MODULE_CONTENT)
endif()
