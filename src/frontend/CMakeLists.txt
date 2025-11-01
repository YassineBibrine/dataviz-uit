cmake_minimum_required(VERSION 3.21)
project(FrontendEngine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(FRONTEND_SOURCES
    ui/main_window.cpp
    ui/main_window.h
    ui/code_editor.cpp
    ui/code_editor.h
    ui/visualization_pane.cpp
    ui/visualization_pane.h
    ui/control_panel.cpp
    ui/control_panel.h
    ui/diagnostic_output.cpp
    ui/diagnostic_output.h
    ui/memory_viewer.cpp
    ui/memory_viewer.h
)

# Only build if Qt is available
if(Qt6_FOUND)
    add_library(frontend_engine ${FRONTEND_SOURCES})

    target_include_directories(frontend_engine PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(frontend_engine
   Qt6::Core
  Qt6::Gui
        Qt6::Widgets
    )
else()
    message(WARNING "Qt6 not found - Frontend engine will not be built")
  # Create stub library
    add_library(frontend_engine INTERFACE)
endif()
