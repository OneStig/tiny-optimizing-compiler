#include <lexing/Lexer.h>

Token Lexer::nextToken() {
    while (isspace(curChar)) {
        next();
    }

    if (curChar == EOF) {
        return Token(TokenType::END_OF_FILE, globalLine, globalCol);
    }

    // Number literals
    if (isdigit(curChar)) {
        return Token(TokenType::NUM, globalLine, globalCol, "", number());
    }

    // Keywords and identifiers
    if (isalpha(curChar)) {
        const std::string name = identifier();
        return Token(keywords.contains(name) ? TokenType::KEYWORD : TokenType::IDENT, globalLine, globalCol, name);
    }

    if (punctuation.contains(curChar)) {
        const std::string puncStr(1, curChar);
        next();
        return Token(TokenType::PUNCTUATION, globalLine, globalCol, puncStr);
    }

    if (genericOperators.contains(curChar)) {
        const std::string opStr(1, curChar);
        next();
        return Token(TokenType::GENOP, globalLine, globalCol, opStr);
    }

    if (curChar == '>') {
        next();
        if (curChar == '=') {
            next();
            return Token(TokenType::RELOP, globalLine, globalCol, ">=");
        }
        return Token(TokenType::RELOP, globalLine, globalCol, ">");
    }

    // Checking if '<' is relational or assignment
    if (curChar == '<') {
        next();
        if (curChar == '=') {
            next();
            return Token(TokenType::RELOP, globalLine, globalCol, "<=");
        }

        if (curChar == '-') {
            next();
            return Token(TokenType::ASSIGN, globalLine, globalCol);
        }

        return Token(TokenType::RELOP, globalLine, globalCol, "<");
    }

    if (curChar == '=' || curChar == '!') {
        const char tmp = curChar;
        next();

        if (curChar == '=') {
            next();
            return Token(TokenType::RELOP, globalLine, globalCol, std::string(1, tmp) + "=");
        }
    }

    throw TinySyntaxError("Syntax error");
}

char Lexer::next() {
    curChar = fr.nextChar();

    if (curChar == '\n') {
        globalLine++;
        globalCol = 0;
    }
    else {
        globalCol++;
    }

    return curChar;
}

int Lexer::number() {
    int buffer = curChar - '0';
    while (isdigit(next())) {
        buffer = buffer * 10 + curChar - '0';
    }

    return buffer;
}

std::string Lexer::identifier() {
    std::string buffer;

    while (isalpha(curChar) || isdigit(curChar)) {
        buffer += curChar;
        next();
    }

    return buffer;
}
