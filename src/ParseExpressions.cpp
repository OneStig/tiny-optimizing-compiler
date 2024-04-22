#include "Parser.h"
#include "AST/Computation.h"
#include "AST/Expression.h"

AST::ASTPtr Parser::factor() {
    auto curNode = std::make_unique<AST::Factor>();

    if (curToken.type == TokenType::IDENT) {
        curNode->ident = curToken.name;
    }
    else if (curToken.type == TokenType::NUM) {
        curNode->value = curToken.value;
    }
    else if (match(curToken, TokenType::PUNCTUATION, "(")) {
        next(); // consume "("
        curNode->append(expression());
        next(); // consume ")"
    }
    else {
        // Syntax error
        return nullptr;
    }

    return curNode;
}

AST::ASTPtr Parser::term() {
    auto curNode = std::make_unique<AST::Term>();
    // factor
    curNode->append(factor());

    // {("*" | "/") factor}.

    while (match(curToken, TokenType::GENOP, "*") ||
        match(curToken, TokenType::GENOP, "/")) {
        curNode->divide.push_back(curToken.name == "/");
        next(); // consume ("*" | "/")

        curNode->append(factor());
    }

    return curNode;
}

AST::ASTPtr Parser::expression() {
    auto curNode = std::make_unique<AST::Expression>();
    // term
    curNode->append(term());

    // {("+" | "-") term}.

    while (match(curToken, TokenType::GENOP, "+") ||
        match(curToken, TokenType::GENOP, "-")) {
        curNode->negate.push_back(curToken.name == "-");
        next(); // consume ("+" | "-")

        curNode->append(term());
    }

    return curNode;
}
AST::ASTPtr Parser::relation() {
    // unimplemented
    return nullptr;
}
