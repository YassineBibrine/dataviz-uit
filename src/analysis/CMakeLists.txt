cmake_minimum_required(VERSION 3.21)
project(AnalysisEngine)

set(ANALYSIS_SOURCES
    clang_integration/ast_parser.h
    clang_integration/ast_parser.cpp
    clang_integration/structure_extractor.h
    clang_integration/structure_extractor.cpp
    instrumentation/llvm_instrumentation.h
    instrumentation/llvm_instrumentation.cpp
    state_mapping/execution_logger.h
    state_mapping/execution_logger.cpp
    state_mapping/state_mapper.h
    state_mapping/state_mapper.cpp
)

if(LLVM_FOUND)
  add_library(analysis_engine ${ANALYSIS_SOURCES})

    target_include_directories(analysis_engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LLVM_INCLUDE_DIRS}
    )

    target_compile_definitions(analysis_engine PRIVATE ${LLVM_DEFINITIONS})

    target_link_libraries(analysis_engine
  ${LLVM_LIBRARIES}
        clang
 )
else()
    message(WARNING "LLVM/Clang not found - Analysis engine will not be built")
    add_library(analysis_engine INTERFACE)
endif()
