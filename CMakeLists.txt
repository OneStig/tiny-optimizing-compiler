cmake_minimum_required(VERSION 3.28)
project(tiny_optimizing_compiler)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(tiny_optimizing_compiler src/main.cpp
        include/lexing/Token.h
        include/lexing/FileReader.h
        include/lexing/Lexer.h
        src/parsing/Parser.cpp
        include/parsing/Parser.h
        include/synthesis/BasicBlock.h
        include/synthesis/Instruction.h
        src/graph/DOTGraph.cpp
        include/graph/DOTGraph.h
        include/parsing/AST/ASTNode.h
        include/parsing/AST/Statements.h
        include/parsing/AST/Declarations.h
        src/parsing/ParseStatements.cpp
        src/parsing/ParseExpressions.cpp
        include/parsing/AST/Expressions.h
        src/synthesis/IRBuilder.cpp
        include/synthesis/IRBuilder.h
        src/lexing/Lexer.cpp
        include/parsing/AST/ASTMaster.h
        src/synthesis/Statements.cpp
        src/synthesis/Expressions.cpp
        src/synthesis/Declarations.cpp
        src/synthesis/IfStatement.cpp
        src/synthesis/WhileStatement.cpp
        include/lexing/Tiny.h
)

enable_testing()

file(GLOB TEST_FILES ./samples/*.tiny)

# Glob files from the public repo
file(GLOB PUBLIC_FILES ./public-samples/*.tiny ./public-samples/*.tny ./public-samples/*.ty ./public-samples/*.txt)
list(APPEND TEST_FILES ${PUBLIC_FILES})

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    if(NOT TEST_NAME STREQUAL "***signupsheet")
        add_test(NAME ${TEST_NAME}
                COMMAND tiny_optimizing_compiler ${TEST_FILE})
        set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 10)
    endif()
endforeach()
