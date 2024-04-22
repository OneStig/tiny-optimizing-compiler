#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include "BasicBlock.h"
#include "Lexer.h"
#include "AST/ASTNode.h"

class Parser {
private:
    Lexer& lx;
    Token curToken;

    int factor();
    int term();
    int expression();
    int relation();

    // Statements:

    void assignment();
    void funcCall();
    void ifStatement();
    void whileStatement();
    void returnStatement();

    AST::ASTPtr statement();
    AST::ASTPtr statSequence();

    AST::ASTPtr varDecl();
    AST::ASTPtr funcDecl();
    AST::ASTPtr formalParam();
    AST::ASTPtr funcBody();
    AST::ASTPtr computation();

    void next() {
        curToken = lx.nextToken();
    }

    static bool match(const Token& token, const TokenType type, const std::string& name = "") {
        return token.type == type && token.name == name;
    }

public:
    explicit Parser(Lexer& lexer) : lx{lexer}, curToken{lx.nextToken()} {
        computation();
    }
};

#endif //PARSER_H
