#include "Parser.h"
#include "AST/Computation.h"

AST::ASTPtr Parser::varDecl() {
    next(); // Consume 'var'

    auto curNode = std::make_unique<AST::VarDecl>();

    // assuming identifier ADD CHECK LATER
    curNode->variables.push_back(curToken.name);
    next(); // consume first identifier

    while (match(curToken, TokenType::PUNCTUATION, ",")) {
        next(); // consume ","
        curNode->variables.push_back(curToken.name);
        next(); // consume ident
    }

    next(); // consume ";"

    return curNode;
}

AST::ASTPtr Parser::funcDecl() {
    next(); // Consume 'void' or 'function'
    return nullptr;
}

AST::ASTPtr Parser::computation() {
    next(); // Consume 'main'

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
        next(); // Consume '{'
        curNode->append(statSequence());
        next(); // Consume '}'
    }

    next(); // Consume '.'

    return curNode;
}
