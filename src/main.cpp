#include <iostream>

#include "FileReader.h"
#include "Lexer.h"

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <tiny source file>" << std::endl;
        return 1;
    }

    FileReader fr(argv[1]);
    Lexer lexer(fr);

    Token test = lexer.nextToken();
    while (test.type != TokenType::END_OF_FILE) {
        std::cout << test.toString() << std::endl;
        test = lexer.nextToken();
    }

    return 0;
}