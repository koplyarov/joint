macro(AUTO_FOLDER)
    file(RELATIVE_PATH __AUTO_FOLDER_RELATIVE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(TARGET_NAME ${ARGN})
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${__AUTO_FOLDER_RELATIVE_PATH})
    endforeach()
endmacro()

macro(AUTO_SOURCE_GROUP)
    file(RELATIVE_PATH __AUTO_FOLDER_RELATIVE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

    set(__AUTO_SOURCE_GROUPS "")
    foreach(FILENAME ${ARGN})
        get_filename_component(__AUTO_SOURCE_GROUP ${FILENAME} DIRECTORY)
        if("${__AUTO_SOURCE_GROUP}" STREQUAL "")
            set(__AUTO_SOURCE_GROUP "root")
        endif()

        list(FIND __AUTO_SOURCE_GROUPS "${__AUTO_SOURCE_GROUP}" __AUTO_SOURCE_GROUP_VISITED)
        if(__AUTO_SOURCE_GROUP_VISITED EQUAL -1)
            list(APPEND __AUTO_SOURCE_GROUPS "${__AUTO_SOURCE_GROUP}")

            set(__AUTO_SOURCE_GROUP_SOURCES  "")
            foreach(FILENAME2 ${ARGN})
                get_filename_component(__AUTO_SOURCE_GROUP2 ${FILENAME2} DIRECTORY)
                if("${__AUTO_SOURCE_GROUP2}" STREQUAL "")
                    set(__AUTO_SOURCE_GROUP2 "root")
                endif()

                if("${__AUTO_SOURCE_GROUP}" STREQUAL "${__AUTO_SOURCE_GROUP2}")
                    list(APPEND __AUTO_SOURCE_GROUP_SOURCES "${FILENAME2}")
                endif()
            endforeach()

            if(NOT "${__AUTO_SOURCE_GROUP}" STREQUAL "")
                string(REPLACE "/" "\\" __AUTO_SOURCE_GROUP "${__AUTO_SOURCE_GROUP}")
                source_group(${__AUTO_SOURCE_GROUP} FILES ${__AUTO_SOURCE_GROUP_SOURCES})
            endif()
        endif()
    endforeach()
endmacro()