#include "parsing/Parser.h"
#include "parsing/AST/Expressions.h"

AST::ASTPtr Parser::factor() {
    auto curNode = std::make_unique<AST::Factor>();

    if (curToken.type == TokenType::IDENT) {
        auto identNode = std::make_unique<AST::Identifier>();
        identNode->name = curToken.name;
        curNode->append(std::move(identNode));

        next(); // consume ident
    }
    else if (curToken.type == TokenType::NUM) {
        auto numNode = std::make_unique<AST::Number>();
        numNode->value = curToken.value;
        curNode->append(std::move(numNode));
        next(); // consume number
    }
    else if (match(curToken, TokenType::PUNCTUATION, "(")) {
        next(); // consume "("
        curNode->append(expression());
        next(); // consume ")"
    }
    else if (match(curToken, TokenType::KEYWORD, "call")) {
        curNode->append(funcCall());
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

std::unique_ptr<AST::Relation> Parser::relation() {
    auto curNode = std::make_unique<AST::Relation>();

    curNode->append(expression());

    // expect next token to be relOp
    // (conditions are flipped because we branch on else)
    if (curToken.name == "!=") curNode->relType = InsType::BEQ;
    else if (curToken.name == "==") curNode->relType = InsType::BNE;
    else if (curToken.name == "<=") curNode->relType = InsType::BGT;
    else if (curToken.name == "<") curNode->relType = InsType::BGE;
    else if (curToken.name == ">=") curNode->relType = InsType::BLT;
    else if (curToken.name == ">") curNode->relType = InsType::BLE;
    else curNode->relType = InsType::BRA;
    next(); // consume relOp

    curNode->append(expression());

    return curNode;
}
