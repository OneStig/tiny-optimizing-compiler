cmake_minimum_required(VERSION 3.28)
project(tiny_optimizing_compiler)

set(CMAKE_CXX_STANDARD 20)

set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/lib/pkgconfig:/opt/homebrew/Cellar/graphviz/10.0.1/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")

find_package(PkgConfig REQUIRED)
pkg_search_module(GRAPHVIZ REQUIRED libgvc libcdt libgraph)

include_directories(${GRAPHVIZ_INCLUDE_DIRS})
link_directories(${GRAPHVIZ_LIBRARY_DIRS})

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
        include/parsing/AST/Statement.h
        include/parsing/AST/Computation.h
        src/parsing/ParseStatements.cpp
        include/parsing/AST/StatSequence.h
        include/parsing/AST/Assignment.h
        src/parsing/ParseExpressions.cpp
        include/parsing/AST/Expression.h
        include/parsing/AST/FuncCall.h
        src/synthesis/IRBuilder.cpp
        include/synthesis/IRBuilder.h
        src/lexing/Lexer.cpp
        include/parsing/AST/ASTMaster.h
)

target_link_libraries(tiny_optimizing_compiler ${GRAPHVIZ_LIBRARIES})