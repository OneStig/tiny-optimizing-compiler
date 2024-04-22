#include "Parser.h"
#include "AST/Computation.h"

AST::ASTPtr Parser::computation() {
    lx.nextToken(); // Consume 'main'

    AST::ASTPtr curNode = std::make_unique<AST::Computation>();

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
        lx.nextToken(); // Consume '{'
        curNode->append(statSequence());
        lx.nextToken(); // Consume '}'
    }

    lx.nextToken(); // Consume '.'

    return curNode;
}
