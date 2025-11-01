cmake_minimum_required(VERSION 3.21)
project(CoreLibrary)

set(CORE_SOURCES
    data_structures/graph.h
    data_structures/graph.cpp
    data_structures/tree.h
    data_structures/tree.cpp
    serialization/dot_serializer.h
    serialization/dot_serializer.cpp
    serialization/json_serializer.h
    serialization/json_serializer.cpp
    utilities/logger.h
    utilities/logger.cpp
    utilities/timer.h
    utilities/timer.cpp
)

if(Boost_FOUND)
    add_library(core_lib ${CORE_SOURCES})

    target_include_directories(core_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
    )

    target_link_libraries(core_lib
        ${Boost_LIBRARIES}
    )
else()
    message(WARNING "Boost not found - Core library will not be built")
    add_library(core_lib INTERFACE)
endif()
