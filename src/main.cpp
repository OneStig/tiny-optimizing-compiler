#include <cstdlib>
#include <iostream>
#include <filesystem>

#include "lexing/FileReader.h"
#include "lexing/Lexer.h"
#include "parsing/Parser.h"
#include "synthesis/IRBuilder.h"
#include "graph/DOTGraph.h"

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <tiny source file>" << std::endl;
        return 1;
    }

    std::filesystem::path filePath = argv[1];

    // Read source file and tokenize contents
    FileReader fr(argv[1]);
    Lexer lexer(fr);

    // Parse to produce AST
    Parser parser(lexer);

    // Build IR
    IRBuilder builder;
    int startBlock = 0;
    parser.ast->evaluate(builder, startBlock);
    builder.cleanUp(true, true);

    // Generate dot graph
    DOTGraph g(builder.blocks, filePath.stem());
    g.visualize();

    return 0;
}
