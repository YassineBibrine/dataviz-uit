cmake_minimum_required(VERSION 3.16)
project(dataviz-uit VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

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
    src/core/DataModelManager.cpp
    
  # Orchestration
    src/orchestration/algorithm_manager.cpp
    
    # Algorithms
    src/algorithms/algorithm_runner.cpp
    src/algorithms/execution_state.cpp
    
    # Visualization
    src/visualization/GraphvizLayoutEngine.cpp
    src/visualization/animation_frame.cpp
    src/visualization/visualization_renderer.cpp
    src/visualization/interaction_manager.cpp
    
    # UI
    src/ui/main_window.cpp
    src/ui/visualization_pane.cpp
    src/ui/control_panel.cpp
    src/ui/metrics_panel.cpp
)

# Header files
set(HEADERS
    # Include headers
    include/core/DataStructure.h
    include/core/AlgorithmInfo.h
    include/core/AlgorithmFactory.h
    include/core/AlgorithmManager.h
    include/core/DataModelManager.h
    include/execution/AlgorithmRunner.h
    include/execution/ExecutionState.h
    include/execution/FrameRecorder.h
    include/execution/algorithms/BFSRunner.h
    include/core/factories/BFSFactory.h
    include/visualization/GraphvizLayoutEngine.h
    include/visualization/VisualizationRenderer.h
    include/visualization/PlaybackController.h
    include/visualization/InteractionManager.h
    include/visualization/AnimationFrame.h
    include/ui/MainWindow.h
    include/ui/VisualizationPane.h
    include/ui/ControlPanel.h
    include/ui/MetricsPanel.h
    
    # Src headers
    src/core/graph.h
    src/core/list_node.h
    src/core/tree_node.h
    src/orchestration/algorithm_factory.h
    src/orchestration/algorithm_manager.h
    src/algorithms/algorithm_runner.h
    src/algorithms/execution_state.h
    src/algorithms/frame_recorder.h
 src/visualization/animation_frame.h
    src/visualization/visualization_renderer.h
    src/visualization/interaction_manager.h
    src/ui/main_window.h
    src/ui/visualization_pane.h
    src/ui/control_panel.h
    src/ui/metrics_panel.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
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
