#include <cstdlib>
#include <iostream>

#include "FileReader.h"
#include "../include/lexing/Lexer.h"
#include "Parser.h"
#include "IRBuilder.h"
#include "graph/DOTGraph.h"

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <tiny source file>" << std::endl;
        return 1;
    }

    // Read source file and tokenize contents
    FileReader fr(argv[1]);
    Lexer lexer(fr);

    // Parse to produce AST
    Parser parser(lexer);

    // Build IR
    IRBuilder builder;
    int startBlock = 0;
    parser.ast->evaluate(builder, startBlock);

    // Generate dot graph
    DOTGraph g(builder.blocks);
    g.visualize();

    return 0;
}
