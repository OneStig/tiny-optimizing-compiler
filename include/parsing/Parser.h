#ifndef PARSER_H
#define PARSER_H

#include <memory>

#include "AST/ASTMaster.h"
#include "lexing/Lexer.h"

class Parser {
private:
    Lexer& lx;
    Token curToken;

    AST::ASTPtr factor();
    AST::ASTPtr term();
    AST::ASTPtr expression();
    std::unique_ptr<AST::Relation> relation();

    // Statements:

    std::unique_ptr<AST::Assignment> assignment();
    AST::ASTPtr funcCall();
    AST::ASTPtr ifStatement();
    AST::ASTPtr whileStatement();
    AST::ASTPtr returnStatement();

    AST::ASTPtr statement();
    AST::ASTPtr statSequence();

    AST::ASTPtr varDecl();
    AST::ASTPtr funcDecl();
    AST::ASTPtr formalParam();
    AST::ASTPtr funcBody();
    AST::ASTPtr computation();

    void next() {
        curToken = lx.nextToken();
        // std::cout << "Advance Token to: " << curToken.toString() << std::endl;
    }

    static bool match(const Token& token, const TokenType type, const std::string& name = "") {
        return token.type == type && token.name == name;
    }

public:
    AST::ASTPtr ast;

    explicit Parser(Lexer& lexer) : lx{lexer}, curToken{lx.nextToken()} {
        ast = computation();
    }
};

#endif //PARSER_H
