#include <iostream>

#include "FileReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "IRBuilder.h"

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <tiny source file>" << std::endl;
        return 1;
    }

    // Read source file
    FileReader fr(argv[1]);

    // Tokenize contents
    Lexer lexer(fr);

    // Token test = lexer.nextToken();
    // int i = 1;
    // while (test.type != TokenType::END_OF_FILE) {
    //     std::cout << i++ << test.toString() << std::endl;
    //     test = lexer.nextToken();
    // }

    // Parse to produce AST
    Parser parser(lexer);

    // Build IR
    IRBuilder builder;
    parser.ast->evaluate(builder, 0);

    return 0;
}
