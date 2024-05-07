#include "parsing/Parser.h"
#include "parsing/AST/Declarations.h"

AST::ASTPtr Parser::varDecl() {
    next(TokenType::KEYWORD); // Consume 'var'

    auto curNode = std::make_unique<AST::VarDecl>();

    // assuming identifier ADD CHECK LATER
    curNode->variables.push_back(curToken.name);
    next(TokenType::IDENT); // consume first identifier

    while (match(curToken, TokenType::PUNCTUATION, ",")) {
        next(TokenType::PUNCTUATION); // consume ","
        curNode->variables.push_back(curToken.name);
        next(TokenType::IDENT); // consume ident
    }

    next(TokenType::PUNCTUATION); // consume ";"

    return curNode;
}

AST::ASTPtr Parser::funcDecl() {
    next(TokenType::KEYWORD); // Consume 'void' or 'function'
    return nullptr;
}

AST::ASTPtr Parser::computation() {
    next(TokenType::KEYWORD); // Consume 'main'

    auto curNode = std::make_unique<AST::Computation>();

    // [varDecl]
    if (match(curToken, TokenType::KEYWORD, "var")) {
        curNode->append(varDecl());
    }

    // {funcDecl}
    while (match(curToken, TokenType::KEYWORD, "void") |
        match(curToken, TokenType::KEYWORD, "function")) {
        curNode->append(funcDecl());
    }

    // "{" statSequence "}" "."
    if (match(curToken, TokenType::PUNCTUATION, "{")) {
        next(TokenType::PUNCTUATION); // Consume '{'
        curNode->append(statSequence());
        next(TokenType::PUNCTUATION); // Consume '}'
    }

    next(TokenType::PUNCTUATION); // Consume '.'

    return curNode;
}
