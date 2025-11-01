cmake_minimum_required(VERSION 3.21)
project(SynthesisEngine)

set(SYNTHESIS_SOURCES
    code_generation/clang_rewriter.h
    code_generation/clang_rewriter.cpp
    code_generation/ast_generator.h
    code_generation/ast_generator.cpp
    code_generation/code_formatter.h
    code_generation/code_formatter.cpp
    api_mapping/api_mapper.h
    api_mapping/api_mapper.cpp
)

if(LLVM_FOUND)
    add_library(synthesis_engine ${SYNTHESIS_SOURCES})

    target_include_directories(synthesis_engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LLVM_INCLUDE_DIRS}
    )

    target_compile_definitions(synthesis_engine PRIVATE ${LLVM_DEFINITIONS})

    target_link_libraries(synthesis_engine
        ${LLVM_LIBRARIES}
        clang
    )
else()
    message(WARNING "LLVM/Clang not found - Synthesis engine will not be built")
    add_library(synthesis_engine INTERFACE)
endif()
