cmake_minimum_required(VERSION 3.16)
project(dataviz-uit VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# === Graphviz Configuration (Windows) ===
set(GRAPHVIZ_FOUND FALSE)
if(WIN32)
    set(GRAPHVIZ_ROOT "C:/Program Files/Graphviz")
    if(EXISTS "${GRAPHVIZ_ROOT}/include/graphviz/gvc.h")
  set(GRAPHVIZ_FOUND TRUE)
        message(STATUS "Graphviz found at: ${GRAPHVIZ_ROOT}")
        
   # Include headers
        include_directories("${GRAPHVIZ_ROOT}/include/graphviz")
        
   # Define DLL paths
  set(GRAPHVIZ_GVC_DLL "${GRAPHVIZ_ROOT}/bin/gvc.dll")
        set(GRAPHVIZ_CGRAPH_DLL "${GRAPHVIZ_ROOT}/bin/cgraph.dll")
        set(GRAPHVIZ_CDT_DLL "${GRAPHVIZ_ROOT}/bin/cdt.dll")
    else()
   message(WARNING "Graphviz not found at ${GRAPHVIZ_ROOT}. Graphviz features will be disabled.")
    endif()
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/main.cpp
    
    # Core
    src/core/graph.cpp
    src/core/list_node.cpp
    src/core/tree_node.cpp
    src/core/data_model_manager.cpp
    src/core/array_structure.cpp
    src/core/graph_structure.cpp
    src/core/list_structure.cpp
    src/core/tree_structure.cpp
    
    # Orchestration
    src/orchestration/algorithm_manager.cpp
    
    # Algorithms
    src/algorithms/algorithm_runner.cpp
    src/algorithms/execution_state.cpp
    
    # Visualization
    src/visualization/animation_frame.cpp
    src/visualization/visualization_renderer.cpp
    src/visualization/interaction_manager.cpp
    src/visualization/playback_controller.cpp

    # UI
    src/ui/main_window.cpp
    src/ui/visualization_pane.cpp
    src/ui/control_panel.cpp
    src/ui/metrics_panel.cpp
)

# Header files
set(HEADERS
    # Src headers
    src/core/graph.h
    src/core/list_node.h
    src/core/tree_node.h
    src/core/data_model_manager.h
    src/core/data_structure.h
    src/core/algorithm_info.h
    src/core/array_structure.h
    src/core/graph_structure.h
    src/core/list_structure.h
src/core/tree_structure.h
    src/orchestration/algorithm_factory.h
    src/orchestration/algorithm_manager.h
    src/algorithms/algorithm_runner.h
    src/algorithms/execution_state.h
    src/algorithms/frame_recorder.h
    src/visualization/animation_frame.h
    src/visualization/visualization_renderer.h
    src/visualization/interaction_manager.h
    src/visualization/playback_controller.h
    src/ui/main_window.h
    src/ui/visualization_pane.h
    src/ui/control_panel.h
    src/ui/metrics_panel.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "src/ui/toolbox_panel.h" "src/ui/toolbox_panel.cpp")

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
    gvc
    cgraph
    cdt
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Platform-specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
 )
    
    # === Copy Graphviz DLLs to output directory (if found) ===
    if(GRAPHVIZ_FOUND)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
 "${GRAPHVIZ_GVC_DLL}"
"${GRAPHVIZ_CGRAPH_DLL}"
        "${GRAPHVIZ_CDT_DLL}"
     $<TARGET_FILE_DIR:${PROJECT_NAME}>
      COMMENT "Copying Graphviz DLLs to output directory"
   )
    endif()
endif()

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
