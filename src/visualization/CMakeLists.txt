cmake_minimum_required(VERSION 3.21)
project(VisualizationEngine)

set(VISUALIZATION_SOURCES
    graphviz/graphviz_layout_engine.h
    graphviz/graphviz_layout_engine.cpp
    rendering/dot_renderer.h
    rendering/dot_renderer.cpp
    rendering/visualization_builder.h
    rendering/visualization_builder.cpp
    animation/animation_controller.h
    animation/animation_controller.cpp
)

if(Boost_FOUND)
    add_library(visualization_engine ${VISUALIZATION_SOURCES})

    target_include_directories(visualization_engine PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
    )

  target_link_libraries(visualization_engine
      core_lib
    )
else()
    message(WARNING "Dependencies not found - Visualization engine will not be built")
    add_library(visualization_engine INTERFACE)
endif()
