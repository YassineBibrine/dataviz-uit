cmake_minimum_required(VERSION 3.14)
project(dataviz-uit VERSION 1.0 LANGUAGES CXX)

# ===================================================================
# C++ Standard Configuration - Portable across compilers
# ===================================================================
# Require C++17, but gracefully handle older compilers
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable position independent code for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Always generate compile_commands.json for IDE IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ===================================================================
# Compiler-specific flags for portability
# ===================================================================
if(MSVC)
    # MSVC specific flags
    add_compile_options(/W3 /permissive-)
    # Suppress specific warnings that may occur with different MSVC versions
    add_compile_options(/wd4996)  # Deprecated functions
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(NOMINMAX)  # Prevent Windows.h min/max macros
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC/Clang flags
    add_compile_options(-Wall -Wextra -Wpedantic)
    # Suppress specific warnings for portability
    add_compile_options(-Wno-unused-parameter)
endif()

# ===================================================================
# Qt Configuration - Supports Qt5 and Qt6
# ===================================================================
# Priority 1: User-defined CMAKE_PREFIX_PATH (from command line)
# Priority 2: QT_DIR or Qt6_DIR/Qt5_DIR environment variable
# Priority 3: Common installation paths (fallback)
# ===================================================================

# Determine Qt major version to use (prefer Qt6, fallback to Qt5)
set(QT_VERSION_MAJOR 6)
option(USE_QT5 "Force use of Qt5 instead of Qt6" OFF)
if(USE_QT5)
  set(QT_VERSION_MAJOR 5)
endif()

# Setup Qt search paths
if(WIN32)
    if(NOT DEFINED CMAKE_PREFIX_PATH OR CMAKE_PREFIX_PATH STREQUAL "")
        set(QT_SEARCH_PATHS "")

        # Check environment variables first
        if(DEFINED ENV{QT_DIR} AND EXISTS "$ENV{QT_DIR}")
 list(APPEND QT_SEARCH_PATHS "$ENV{QT_DIR}")
            message(STATUS "Found QT_DIR environment variable: $ENV{QT_DIR}")
        endif()
        if(DEFINED ENV{Qt6_DIR} AND EXISTS "$ENV{Qt6_DIR}")
   list(APPEND QT_SEARCH_PATHS "$ENV{Qt6_DIR}")
        message(STATUS "Found Qt6_DIR environment variable: $ENV{Qt6_DIR}")
        endif()
        if(DEFINED ENV{Qt5_DIR} AND EXISTS "$ENV{Qt5_DIR}")
      list(APPEND QT_SEARCH_PATHS "$ENV{Qt5_DIR}")
   message(STATUS "Found Qt5_DIR environment variable: $ENV{Qt5_DIR}")
        endif()

        # Common Qt6 installation paths
   list(APPEND QT_SEARCH_PATHS
  "C:/Qt/6.10.0/msvc2022_64"
   "C:/Qt/6.9.0/msvc2022_64"
      "C:/Qt/6.8.3/msvc2022_64"
            "C:/Qt/6.8.2/msvc2022_64"
            "C:/Qt/6.8.1/msvc2022_64"
            "C:/Qt/6.8.0/msvc2022_64"
    "C:/Qt/6.7.3/msvc2022_64"
         "C:/Qt/6.7.2/msvc2022_64"
            "C:/Qt/6.7.1/msvc2022_64"
  "C:/Qt/6.7.0/msvc2022_64"
   "C:/Qt/6.6.3/msvc2022_64"
            "C:/Qt/6.6.2/msvc2022_64"
     "C:/Qt/6.6.1/msvc2022_64"
            "C:/Qt/6.6.0/msvc2022_64"
      "C:/Qt/6.5.3/msvc2019_64"
        "C:/Qt/6.5.2/msvc2019_64"
        "C:/Qt/6.5.1/msvc2019_64"
"C:/Qt/6.5.0/msvc2019_64"
         "C:/Qt/6.4.3/msvc2019_64"
            "C:/Qt/6.4.2/msvc2019_64"
   "C:/Qt/6.4.1/msvc2019_64"
     "C:/Qt/6.4.0/msvc2019_64"
          # MinGW variants
     "C:/Qt/6.8.0/mingw_64"
            "C:/Qt/6.7.0/mingw_64"
      "C:/Qt/6.6.0/mingw_64"
      # Qt5 paths (fallback)
 "C:/Qt/5.15.2/msvc2019_64"
      "C:/Qt/5.15.2/msvc2017_64"
            "C:/Qt/5.15.0/msvc2019_64"
       "C:/Qt/5.14.2/msvc2017_64"
    "C:/Qt/5.12.12/msvc2017_64"
        )

        # Find first existing Qt installation
      set(QT_FOUND_PATH "")
     foreach(qt_path IN LISTS QT_SEARCH_PATHS)
     if(EXISTS "${qt_path}")
    set(QT_FOUND_PATH "${qt_path}")
       list(APPEND CMAKE_PREFIX_PATH "${qt_path}")
    message(STATUS "Found Qt at: ${qt_path}")
     break()
            endif()
        endforeach()

        if(QT_FOUND_PATH STREQUAL "")
            message(WARNING
  "\n"
           "================================================================================\n"
   "  Qt NOT FOUND!\n"
      "================================================================================\n"
          "  Please install Qt and configure using one of these methods:\n\n"
 "  Method 1: Set environment variable\n"
       "    Windows: set QT_DIR=C:\\Qt\\6.x.x\\msvc2022_64\n"
    "    Linux:   export QT_DIR=/opt/Qt/6.x.x/gcc_64\n\n"
       "  Method 2: Pass to CMake\n"
   "    cmake -DCMAKE_PREFIX_PATH=\"/path/to/Qt\" ..\n\n"
              "  Download Qt from: https://www.qt.io/download-qt-installer\n"
                "================================================================================\n"
         )
      endif()
    else()
        message(STATUS "Using user-defined CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
    endif()
elseif(APPLE)
    # macOS common paths
    if(NOT DEFINED CMAKE_PREFIX_PATH OR CMAKE_PREFIX_PATH STREQUAL "")
        set(QT_SEARCH_PATHS
        "$ENV{QT_DIR}"
            "$ENV{HOME}/Qt/6.8.0/macos"
            "$ENV{HOME}/Qt/6.7.0/macos"
  "$ENV{HOME}/Qt/6.6.0/macos"
         "/opt/homebrew/opt/qt@6"
            "/usr/local/opt/qt@6"
            "/opt/homebrew/opt/qt"
            "/usr/local/opt/qt"
        )
        foreach(qt_path IN LISTS QT_SEARCH_PATHS)
        if(qt_path AND EXISTS "${qt_path}")
     list(APPEND CMAKE_PREFIX_PATH "${qt_path}")
         message(STATUS "Found Qt at: ${qt_path}")
      break()
      endif()
  endforeach()
    endif()
elseif(UNIX)
    # Linux common paths
    if(NOT DEFINED CMAKE_PREFIX_PATH OR CMAKE_PREFIX_PATH STREQUAL "")
        set(QT_SEARCH_PATHS
        "$ENV{QT_DIR}"
            "$ENV{HOME}/Qt/6.8.0/gcc_64"
            "$ENV{HOME}/Qt/6.7.0/gcc_64"
            "$ENV{HOME}/Qt/6.6.0/gcc_64"
       "/opt/Qt/6.8.0/gcc_64"
 "/opt/Qt/6.7.0/gcc_64"
            "/opt/Qt/6.6.0/gcc_64"
            "/usr/lib/qt6"
"/usr/lib/x86_64-linux-gnu/qt6"
        )
        foreach(qt_path IN LISTS QT_SEARCH_PATHS)
            if(qt_path AND EXISTS "${qt_path}")
     list(APPEND CMAKE_PREFIX_PATH "${qt_path}")
   message(STATUS "Found Qt at: ${qt_path}")
   break()
            endif()
   endforeach()
    endif()
endif()

# Enable Qt auto-processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Try to find Qt6 first, then fall back to Qt5
set(QT_FOUND OFF)
set(QT_VERSION_STRING "")

# Try Qt6
find_package(Qt6 QUIET COMPONENTS Core Gui Widgets)
if(Qt6_FOUND)
    set(QT_FOUND ON)
    set(QT_VERSION_MAJOR 6)
    set(QT_VERSION_STRING "${Qt6_VERSION}")
    set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets)
    message(STATUS "Found Qt6 version ${Qt6_VERSION}")
endif()

# Fall back to Qt5 if Qt6 not found
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
        "Qt not found! Please install Qt5 or Qt6 and set CMAKE_PREFIX_PATH or QT_DIR environment variable."
    )
endif()

# ===================================================================
# Source Files
# ===================================================================
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE DATAVIZ_SOURCES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE DATAVIZ_HEADERS CONFIGURE_DEPENDS "${SRC_DIR}/*.h")

# ===================================================================
# Main Executable
# ===================================================================
add_executable(${PROJECT_NAME} ${DATAVIZ_SOURCES} ${DATAVIZ_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBRARIES})

# ===================================================================
# Graphviz Configuration - Optional dependency
# ===================================================================
option(USE_GRAPHVIZ "Enable Graphviz for advanced graph layouts" ON)

set(GRAPHVIZ_FOUND OFF)
set(GRAPHVIZ_ROOT_DIR "")

if(USE_GRAPHVIZ)
    if(WIN32)
        # Windows: Search for Graphviz
        set(GRAPHVIZ_SEARCH_PATHS
   "$ENV{GRAPHVIZ_ROOT}"
          "$ENV{GRAPHVIZ_HOME}"
            "$ENV{GRAPHVIZ_DIR}"
  "C:/Program Files/Graphviz"
      "C:/Program Files (x86)/Graphviz"
      "C:/Graphviz"
      )

     foreach(candidate_path IN LISTS GRAPHVIZ_SEARCH_PATHS)
          if(candidate_path AND EXISTS "${candidate_path}/include/graphviz/gvc.h")
   set(GRAPHVIZ_ROOT_DIR "${candidate_path}")
              set(GRAPHVIZ_FOUND ON)
           break()
            endif()
    endforeach()

      if(GRAPHVIZ_FOUND)
        message(STATUS "Found Graphviz at: ${GRAPHVIZ_ROOT_DIR}")

            target_include_directories(${PROJECT_NAME} PRIVATE "${GRAPHVIZ_ROOT_DIR}/include")

       # Find libraries
            set(GRAPHVIZ_LIBS_FOUND ON)
            set(GRAPHVIZ_LIBRARIES "")
       foreach(lib_name gvc cgraph cdt)
              find_library(GRAPHVIZ_${lib_name}_LIB
               NAMES ${lib_name}
      PATHS "${GRAPHVIZ_ROOT_DIR}/lib"
        NO_DEFAULT_PATH
     )
      if(GRAPHVIZ_${lib_name}_LIB)
     list(APPEND GRAPHVIZ_LIBRARIES "${GRAPHVIZ_${lib_name}_LIB}")
          else()
            set(GRAPHVIZ_LIBS_FOUND OFF)
  message(STATUS "Graphviz library ${lib_name} not found")
         endif()
      endforeach()

      if(GRAPHVIZ_LIBS_FOUND)
 target_link_libraries(${PROJECT_NAME} PRIVATE ${GRAPHVIZ_LIBRARIES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHVIZ_AVAILABLE=1)

      # Copy DLLs
             foreach(dll_name gvc.dll cgraph.dll cdt.dll)
          set(dll_path "${GRAPHVIZ_ROOT_DIR}/bin/${dll_name}")
   if(EXISTS "${dll_path}")
     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_if_different
       "${dll_path}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
       COMMENT "Copying ${dll_name}"
           )
 endif()
  endforeach()
            else()
       set(GRAPHVIZ_FOUND OFF)
   endif()
   endif()

    elseif(APPLE OR UNIX)
        # macOS/Linux: Use pkg-config or find_library
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

        if(NOT GRAPHVIZ_FOUND)
            # Try manual search
       find_path(GRAPHVIZ_INCLUDE_DIR graphviz/gvc.h
   PATHS /usr/include /usr/local/include /opt/homebrew/include
      )
 find_library(GRAPHVIZ_GVC_LIB gvc
       PATHS /usr/lib /usr/local/lib /opt/homebrew/lib
            )
     find_library(GRAPHVIZ_CGRAPH_LIB cgraph
        PATHS /usr/lib /usr/local/lib /opt/homebrew/lib
            )
         find_library(GRAPHVIZ_CDT_LIB cdt
       PATHS /usr/lib /usr/local/lib /opt/homebrew/lib
            )

          if(GRAPHVIZ_INCLUDE_DIR AND GRAPHVIZ_GVC_LIB AND GRAPHVIZ_CGRAPH_LIB AND GRAPHVIZ_CDT_LIB)
                set(GRAPHVIZ_FOUND ON)
       target_include_directories(${PROJECT_NAME} PRIVATE ${GRAPHVIZ_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE
       ${GRAPHVIZ_GVC_LIB} ${GRAPHVIZ_CGRAPH_LIB} ${GRAPHVIZ_CDT_LIB}
    )
       target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHVIZ_AVAILABLE=1)
       message(STATUS "Found Graphviz manually")
     endif()
        endif()
    endif()

    if(NOT GRAPHVIZ_FOUND)
   message(STATUS "Graphviz not found - Using fallback layout engine (this is OK)")
 target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHVIZ_AVAILABLE=0)
    endif()
else()
    message(STATUS "Graphviz disabled by user")
    target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHVIZ_AVAILABLE=0)
endif()

# ===================================================================
# Output Configuration
# ===================================================================
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# ===================================================================
# Installation
# ===================================================================
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# ===================================================================
# Qt Deployment (Windows)
# ===================================================================
if(WIN32 AND QT_FOUND)
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}" NO_DEFAULT_PATH)

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
# Copy compile_commands.json to source dir for IDEs (Ninja/Makefile only)
# ===================================================================
if(CMAKE_EXPORT_COMPILE_COMMANDS AND EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
    file(COPY "${CMAKE_BINARY_DIR}/compile_commands.json" DESTINATION "${CMAKE_SOURCE_DIR}")
endif()

# ===================================================================
# Configuration Summary
# ===================================================================
message(STATUS "")
message(STATUS "================================================================================")
message(STATUS "  DataViz-UIT Build Configuration")
message(STATUS "================================================================================")
message(STATUS "  CMake Version:        ${CMAKE_VERSION}")
message(STATUS "  CMake Generator:      ${CMAKE_GENERATOR}")
message(STATUS "  C++ Compiler:         ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C++ Standard:    C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "  Qt Version:           Qt${QT_VERSION_MAJOR} (${QT_VERSION_STRING})")
message(STATUS "  Graphviz:      ${GRAPHVIZ_FOUND}")
if(GRAPHVIZ_FOUND AND GRAPHVIZ_ROOT_DIR)
    message(STATUS "  Graphviz Location:    ${GRAPHVIZ_ROOT_DIR}")
endif()
message(STATUS "  Output Directory:     ${CMAKE_BINARY_DIR}/bin")
message(STATUS "================================================================================")
message(STATUS "")
