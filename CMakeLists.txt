cmake_minimum_required(VERSION 3.16)
project(dataviz-uit VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Optionnel : Supprime les warnings C4530 (Exceptions)
if(MSVC)
    add_compile_options(/EHsc)
endif()

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Configuration Graphviz
set(GRAPHVIZ_DIR "C:/Program Files/Graphviz") 
include_directories(${GRAPHVIZ_DIR}/include)
link_directories(${GRAPHVIZ_DIR}/lib)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# ==========================================
# LISTE COMPLÈTE DES SOURCES (.cpp)
# ==========================================
set(SOURCES
    src/main.cpp
    
    # --- CORE ---
    src/core/graph.cpp
    src/core/list_node.cpp
    src/core/tree_node.cpp
    src/core/data_model_manager.cpp
    src/core/array_structure.cpp
    src/core/graph_structure.cpp
    src/core/list_structure.cpp
    src/core/tree_structure.cpp
    
    # --- ORCHESTRATION ---
    src/orchestration/algorithm_manager.cpp
    src/orchestration/sorting_algorithm_factory.cpp
    src/orchestration/filtering_algorithm_factory.cpp
    src/orchestration/transform_algorithm_factory.cpp
    
    # --- ALGORITHMS ---
    src/algorithms/algorithm_runner.cpp
    src/algorithms/execution_state.cpp
    src/algorithms/frame_recorder.cpp
    src/algorithms/bubble_sort.cpp
    src/algorithms/quick_sort.cpp
    src/algorithms/merge_sort.cpp
    src/algorithms/bfs_runner.cpp
    src/algorithms/dfs_runner.cpp
    src/algorithms/dijkstra_runner.cpp
    src/algorithms/range_filter.cpp
    src/algorithms/normalize.cpp
    
    # --- VISUALIZATION ---
    src/visualization/animation_frame.cpp
    src/visualization/visualization_renderer.cpp
    src/visualization/interaction_manager.cpp
    src/visualization/playback_controller.cpp

    # --- UI ---
    src/ui/main_window.cpp
    src/ui/visualization_pane.cpp
    src/ui/control_panel.cpp
    src/ui/metrics_panel.cpp
    src/ui/toolbox_panel.cpp
)

# ==========================================
# LISTE COMPLÈTE DES HEADERS (.h)
# ==========================================
set(HEADERS
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
    
    src/orchestration/algorithm.h
    src/orchestration/algorithm_factory.h
    src/orchestration/algorithm_manager.h
    src/orchestration/sorting_algorithm_factory.h
    src/orchestration/filtering_algorithm_factory.h
    src/orchestration/transform_algorithm_factory.h
    
    src/algorithms/algorithm_runner.h
    src/algorithms/execution_state.h
    src/algorithms/frame_recorder.h
    src/algorithms/bubble_sort.h
    src/algorithms/quick_sort.h
    src/algorithms/merge_sort.h
    src/algorithms/bfs_runner.h
    src/algorithms/dfs_runner.h
    src/algorithms/dijkstra_runner.h
    src/algorithms/range_filter.h
    src/algorithms/normalize.h
    
    src/visualization/animation_frame.h
    src/visualization/visualization_renderer.h
    src/visualization/interaction_manager.h
    src/visualization/playback_controller.h
    
    src/ui/main_window.h
    src/ui/visualization_pane.h
    src/ui/control_panel.h
    src/ui/metrics_panel.h
    src/ui/toolbox_panel.h
)

# Create executable (J'ai retiré ${RESOURCES} ici)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

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
endif()

# Install target
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)