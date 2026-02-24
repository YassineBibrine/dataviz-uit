cmake_minimum_required(VERSION 3.14)
project(dataviz-uit VERSION 1.0 LANGUAGES CXX)

# ===================================================================
# C++ Standard Configuration
# ===================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ===================================================================
# Compiler-specific flags
# ===================================================================
if(MSVC)
    add_compile_options(/W3 /permissive-)
    add_compile_options(/wd4996)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(NOMINMAX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-Wno-unused-parameter)
endif()

# ===================================================================
# Qt Configuration
# ===================================================================
# How to tell CMake where Qt is (in order of priority):
#
#   1. Pass it at configure time:
#        cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/platform ..
#
#   2. Set an environment variable before running cmake:
#        export Qt6_DIR=/path/to/Qt/6.x.x/platform/lib/cmake/Qt6   (Linux/macOS)
#        set Qt6_DIR=C:\Qt\6.x.x\msvc2022_64\lib\cmake\Qt6          (Windows)
#
#   3. Install Qt system-wide (e.g. via apt, brew, vcpkg) — found automatically.
#
# No hardcoded paths are used so this works on any machine.
# ===================================================================

option(USE_QT5 "Force use of Qt5 instead of Qt6" OFF)

# Propagate environment-variable hints into CMake prefix path
foreach(env_var Qt6_DIR Qt5_DIR QT_DIR)
    if(DEFINED ENV{${env_var}} AND EXISTS "$ENV{${env_var}}")
        list(APPEND CMAKE_PREFIX_PATH "$ENV{${env_var}}")
        message(STATUS "Appended ${env_var} to CMAKE_PREFIX_PATH: $ENV{${env_var}}")
    endif()
endforeach()

# Enable Qt auto-processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Try Qt6 first, then fall back to Qt5
set(QT_FOUND OFF)
set(QT_VERSION_MAJOR 0)
set(QT_VERSION_STRING "")
set(QT_LIBRARIES "")

if(NOT USE_QT5)
    find_package(Qt6 QUIET COMPONENTS Core Gui Widgets)
    if(Qt6_FOUND)
        set(QT_FOUND ON)
        set(QT_VERSION_MAJOR 6)
        set(QT_VERSION_STRING "${Qt6_VERSION}")
        set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets)
        message(STATUS "Found Qt6 version ${Qt6_VERSION}")
    endif()
endif()

if(NOT QT_FOUND)
    find_package(Qt5 QUIET COMPONENTS Core Gui Widgets)
    if(Qt5_FOUND)
        set(QT_FOUND ON)
        set(QT_VERSION_MAJOR 5)
        set(QT_VERSION_STRING "${Qt5_VERSION}")
        set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets)
        message(STATUS "Found Qt5 version ${Qt5_VERSION}")
    endif()
endif()

if(NOT QT_FOUND)
    message(FATAL_ERROR
        "\n"
        "================================================================================\n"
        "  Qt NOT FOUND\n"
        "================================================================================\n"
        "  Install Qt and point CMake to it using ONE of:\n\n"
        "  Option 1 — CMake flag:\n"
        "    cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/platform ..\n\n"
        "  Option 2 — Environment variable (set before running cmake):\n"
        "    export Qt6_DIR=/path/to/Qt/6.x.x/platform/lib/cmake/Qt6\n\n"
        "  Option 3 — System package manager:\n"
        "    Ubuntu/Debian : sudo apt install qt6-base-dev\n"
        "    macOS (Homebrew): brew install qt\n"
        "    Windows (vcpkg): vcpkg install qt6-base\n\n"
        "  Download Qt installer: https://www.qt.io/download-qt-installer\n"
        "================================================================================\n"
    )
endif()

# ===================================================================
# Source Files (relative to project root)
# ===================================================================
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE DATAVIZ_SOURCES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE DATAVIZ_HEADERS CONFIGURE_DEPENDS "${SRC_DIR}/*.h")

# ===================================================================
# Main Executable
# ===================================================================
add_executable(${PROJECT_NAME} ${DATAVIZ_SOURCES} ${DATAVIZ_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBRARIES})

# ===================================================================
# Graphviz — Optional dependency
# ===================================================================
# How to tell CMake where Graphviz is:
#   cmake -DCMAKE_PREFIX_PATH=/path/to/graphviz ..   (all platforms)
#   export GRAPHVIZ_ROOT=/path/to/graphviz            (environment variable)
#   Or install system-wide and it will be found automatically.
# ===================================================================
option(USE_GRAPHVIZ "Enable Graphviz for advanced graph layouts" ON)

if(USE_GRAPHVIZ)
    set(GRAPHVIZ_FOUND OFF)

    # Allow an env-var hint on all platforms
    if(DEFINED ENV{GRAPHVIZ_ROOT} AND EXISTS "$ENV{GRAPHVIZ_ROOT}")
        list(APPEND CMAKE_PREFIX_PATH "$ENV{GRAPHVIZ_ROOT}")
        message(STATUS "Using GRAPHVIZ_ROOT: $ENV{GRAPHVIZ_ROOT}")
    endif()
    foreach(env_var GRAPHVIZ_HOME GRAPHVIZ_DIR)
        if(DEFINED ENV{${env_var}} AND EXISTS "$ENV{${env_var}}")
            list(APPEND CMAKE_PREFIX_PATH "$ENV{${env_var}}")
            message(STATUS "Using ${env_var}: $ENV{${env_var}}")
        endif()
    endforeach()

    # 1. Try pkg-config (Linux/macOS system installs, Homebrew, etc.)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GRAPHVIZ QUIET libgvc)
        if(GRAPHVIZ_FOUND)
            target_include_directories(${PROJECT_NAME} PRIVATE ${GRAPHVIZ_INCLUDE_DIRS})
            target_link_libraries(${PROJECT_NAME} PRIVATE ${GRAPHVIZ_LIBRARIES})
            target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHVIZ_AVAILABLE=1)
            message(STATUS "Found Graphviz via pkg-config")
        endif()
    endif()

    # 2. Fall back to find_path / find_library (works on all platforms
    #    including Windows when CMAKE_PREFIX_PATH or env vars are set)
    if(NOT GRAPHVIZ_FOUND)
        find_path(GRAPHVIZ_INCLUDE_DIR
            NAMES graphviz/gvc.h
        )
        find_library(GRAPHVIZ_GVC_LIB    NAMES gvc)
        find_library(GRAPHVIZ_CGRAPH_LIB NAMES cgraph)
        find_library(GRAPHVIZ_CDT_LIB    NAMES cdt)

        if(GRAPHVIZ_INCLUDE_DIR AND GRAPHVIZ_GVC_LIB AND
           GRAPHVIZ_CGRAPH_LIB AND GRAPHVIZ_CDT_LIB)
            set(GRAPHVIZ_FOUND ON)
            target_include_directories(${PROJECT_NAME} PRIVATE "${GRAPHVIZ_INCLUDE_DIR}")
            target_link_libraries(${PROJECT_NAME} PRIVATE
                "${GRAPHVIZ_GVC_LIB}"
                "${GRAPHVIZ_CGRAPH_LIB}"
                "${GRAPHVIZ_CDT_LIB}"
            )
            target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHVIZ_AVAILABLE=1)
            message(STATUS "Found Graphviz via find_library")
        endif()
    endif()

    # 3. On Windows: copy DLLs next to the executable so it runs in-place.
    #    The DLL directory is resolved from the import library location at
    #    build time — no hardcoded paths needed.
    if(GRAPHVIZ_FOUND AND WIN32)
        foreach(lib_var GRAPHVIZ_GVC_LIB GRAPHVIZ_CGRAPH_LIB GRAPHVIZ_CDT_LIB)
            if(DEFINED ${lib_var})
                get_filename_component(_lib_dir "${${lib_var}}" DIRECTORY)
                # Sibling bin/ directory (common layout: lib/ + bin/)
                get_filename_component(_root_dir "${_lib_dir}" DIRECTORY)
                get_filename_component(_lib_name "${${lib_var}}" NAME_WE)
                set(_dll "${_root_dir}/bin/${_lib_name}.dll")
                if(EXISTS "${_dll}")
                    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "${_dll}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                        COMMENT "Copying ${_lib_name}.dll"
                    )
                endif()
            endif()
        endforeach()
    endif()

    if(NOT GRAPHVIZ_FOUND)
        message(STATUS "Graphviz not found — using fallback layout engine (this is OK)")
        target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHVIZ_AVAILABLE=0)
    endif()
else()
    message(STATUS "Graphviz disabled by user")
    target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHVIZ_AVAILABLE=0)
endif()

# ===================================================================
# Output Configuration (relative to build tree)
# ===================================================================
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# ===================================================================
# Installation
# ===================================================================
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE  DESTINATION .
)

# ===================================================================
# Qt Deployment (Windows — windeployqt resolved from Qt's own bin/)
# ===================================================================
if(WIN32 AND QT_FOUND)
    get_target_property(_qmake_exec Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_exec}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt
        HINTS "${_qt_bin_dir}"
        NO_DEFAULT_PATH
    )
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-translations
                --no-system-d3d-compiler
                --no-opengl-sw
                "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt..."
        )
    endif()
endif()

# ===================================================================
# Copy compile_commands.json to source root for IDE IntelliSense
# ===================================================================
if(CMAKE_EXPORT_COMPILE_COMMANDS AND NOT MSVC)
    if(EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
        file(COPY "${CMAKE_BINARY_DIR}/compile_commands.json" DESTINATION "${CMAKE_SOURCE_DIR}")
    endif()
endif()

# ===================================================================
# Configuration Summary
# ===================================================================
message(STATUS "")
message(STATUS "================================================================================")
message(STATUS "  DataViz-UIT Build Configuration")
message(STATUS "================================================================================")
message(STATUS "  CMake Version:    ${CMAKE_VERSION}")
message(STATUS "  CMake Generator:  ${CMAKE_GENERATOR}")
message(STATUS "  C++ Compiler:     ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C++ Standard:     C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "  Qt Version:       Qt${QT_VERSION_MAJOR} (${QT_VERSION_STRING})")
message(STATUS "  Graphviz:         ${GRAPHVIZ_FOUND}")
message(STATUS "  Output Dir:       ${CMAKE_BINARY_DIR}/bin")
message(STATUS "================================================================================")
message(STATUS "")