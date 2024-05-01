#include <lexing/Lexer.h>

Token Lexer::nextToken() {
    while (isspace(curChar)) {
        next();
    }

    if (curChar == EOF) {
        return Token(TokenType::END_OF_FILE);
    }

    // Number literals
    if (isdigit(curChar)) {
        return Token(TokenType::NUM, "", number());
    }

    // Keywords and identifiers
    if (isalpha(curChar)) {
        const std::string name = identifier();
        return Token(keywords.contains(name) ? TokenType::KEYWORD : TokenType::IDENT, name);
    }

    if (punctuation.contains(curChar)) {
        const std::string puncStr(1, curChar);
        next();
        return Token(TokenType::PUNCTUATION, puncStr);
    }

    if (genericOperators.contains(curChar)) {
        const std::string opStr(1, curChar);
        next();
        return Token(TokenType::GENOP, opStr);
    }

    if (curChar == '>') {
        next();
        if (curChar == '=') {
            next();
            return Token(TokenType::RELOP, ">=");
        }
        return Token(TokenType::RELOP, ">");
    }

    // Checking if '<' is relational or assignment
    if (curChar == '<') {
        next();
        if (curChar == '=') {
            next();
            return Token(TokenType::RELOP, "<=");
        }

        if (curChar == '-') {
            next();
            return Token(TokenType::ASSIGN);
        }

        return Token(TokenType::RELOP, "<");
    }

    if (curChar == '=' || curChar == '!') {
        const char tmp = curChar;
        next();

        if (curChar == '=') {
            next();
            return Token(TokenType::RELOP, std::string(1, tmp) + "=");
        }
    }

    throw TinySyntaxError("Syntax error");
}

char Lexer::next() {
    curChar = fr.nextChar();
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
