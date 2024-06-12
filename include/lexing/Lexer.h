#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>
#include "Token.h"
#include "FileReader.h"

class Lexer {
public:
    int globalLine{1}, globalCol{0};
    explicit Lexer(FileReader& fr) : fr(fr) {
        curChar = fr.nextChar();
    }

    Token nextToken();

private:
    FileReader& fr;
    char curChar;

    char next();
    int number();

    std::string identifier();
};

#endif //LEXER_H
