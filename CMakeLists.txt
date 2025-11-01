cmake_minimum_required(VERSION 3.21)
project(DataStructureVisualizationApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Optional: Find Qt
find_package(Qt6 COMPONENTS 
    Core 
    Gui 
 Widgets 
    QUIET
)

# Optional: Find Boost
find_package(Boost QUIET
    COMPONENTS 
    system 
    filesystem
  graph
)

# Optional: Find LLVM/Clang
find_package(LLVM QUIET CONFIG)
find_package(Clang QUIET CONFIG)

if(LLVM_FOUND)
    include_directories(${LLVM_INCLUDE_DIRS})
    add_definitions(${LLVM_DEFINITIONS})
endif()

# Message about what was found
message(STATUS "=== Dependency Status ===")
if(Qt6_FOUND)
  message(STATUS "? Qt6 found")
else()
    message(STATUS "? Qt6 NOT found - UI will not build")
    message(STATUS "  Set CMAKE_PREFIX_PATH to Qt6 installation directory")
endif()

if(Boost_FOUND)
    message(STATUS "? Boost found")
else()
    message(STATUS "? Boost NOT found - Core library will not build")
endif()

if(LLVM_FOUND)
    message(STATUS "? LLVM/Clang found")
else()
    message(STATUS "? LLVM/Clang NOT found - Analysis/Synthesis engines will not build")
    message(STATUS "  Set LLVM_DIR to LLVM cmake directory")
endif()

# Add subdirectories for each engine (only if dependencies are available)
if(Qt6_FOUND AND Boost_FOUND)
  add_subdirectory(src/frontend)
endif()

if(Boost_FOUND)
    add_subdirectory(src/core)
endif()

if(LLVM_FOUND)
    add_subdirectory(src/analysis)
    add_subdirectory(src/synthesis)
endif()

if(Boost_FOUND)
    add_subdirectory(src/visualization)
endif()

# Main executable (only if Qt found)
if(Qt6_FOUND AND Boost_FOUND AND LLVM_FOUND)
    add_executable(dataviz-app 
        src/main.cpp
    )

    target_link_libraries(dataviz-app
        frontend_engine
      analysis_engine
        synthesis_engine
        core_lib
    visualization_engine
        Qt6::Core
  Qt6::Gui
    Qt6::Widgets
        ${Boost_LIBRARIES}
  ${LLVM_LIBRARIES}
        clang
    )
    
    message(STATUS "? Building dataviz-app executable")
else()
    message(STATUS "? Cannot build dataviz-app - missing dependencies")
    message(STATUS "  Required: Qt6, Boost, LLVM/Clang")
endif()
