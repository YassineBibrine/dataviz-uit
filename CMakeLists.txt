cmake_minimum_required(VERSION 3.16)
project(dataviz-uit VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE DATAVIZ_SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${DATAVIZ_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

set(GRAPHVIZ_FOUND OFF)
set(GRAPHVIZ_ROOT_DIR "")
set(GRAPHVIZ_DLLS)

if(WIN32)
    set(GRAPHVIZ_SEARCH_PATHS
        "$ENV{GRAPHVIZ_ROOT}"
        "$ENV{GRAPHVIZ_HOME}"
        "C:/Program Files/Graphviz"
        "C:/Program Files (x86)/Graphviz"
    )

    foreach(candidate_path IN LISTS GRAPHVIZ_SEARCH_PATHS)
        if(candidate_path AND EXISTS "${candidate_path}/include/graphviz/gvc.h")
            set(GRAPHVIZ_FOUND ON)
            set(GRAPHVIZ_ROOT_DIR "${candidate_path}")
            break()
        endif()
    endforeach()

    if(GRAPHVIZ_FOUND)
        message(STATUS "Graphviz detected at ${GRAPHVIZ_ROOT_DIR}")
        target_include_directories(${PROJECT_NAME} PRIVATE "${GRAPHVIZ_ROOT_DIR}/include")
        set(GRAPHVIZ_LIBRARY_NAMES gvc cgraph cdt)
        set(GRAPHVIZ_LINK_LIBRARIES)

        foreach(lib_name IN LISTS GRAPHVIZ_LIBRARY_NAMES)
            find_library(GRAPHVIZ_${lib_name}_LIBRARY NAMES ${lib_name}
                PATHS "${GRAPHVIZ_ROOT_DIR}/lib"
                NO_DEFAULT_PATH
            )

            if(GRAPHVIZ_${lib_name}_LIBRARY)
                list(APPEND GRAPHVIZ_LINK_LIBRARIES "${GRAPHVIZ_${lib_name}_LIBRARY}")
            else()
                set(GRAPHVIZ_FOUND OFF)
                message(WARNING "Graphviz library ${lib_name} not found under ${GRAPHVIZ_ROOT_DIR}/lib")
                break()
            endif()
        endforeach()

        if(GRAPHVIZ_FOUND)
            target_link_libraries(${PROJECT_NAME} PRIVATE ${GRAPHVIZ_LINK_LIBRARIES})
            set(GRAPHVIZ_DLL_NAMES gvc.dll cgraph.dll cdt.dll)

            foreach(dll_name IN LISTS GRAPHVIZ_DLL_NAMES)
                set(dll_path "${GRAPHVIZ_ROOT_DIR}/bin/${dll_name}")
                if(EXISTS "${dll_path}")
                    list(APPEND GRAPHVIZ_DLLS "${dll_path}")
                endif()
            endforeach()
        endif()
    else()
        message(WARNING "Graphviz headers not found; the fallback layout will be used.")
    endif()

    if(GRAPHVIZ_DLLS)
        foreach(graphviz_dll IN LISTS GRAPHVIZ_DLLS)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${graphviz_dll}"
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMENT "Copying ${graphviz_dll} to output directory"
            )
        endforeach()
    endif()
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
