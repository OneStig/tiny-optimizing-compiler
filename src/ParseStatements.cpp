#include "Parser.h"
#include "AST/Assignment.h"
#include "AST/Computation.h"
#include "AST/FuncCall.h"
#include "AST/StatSequence.h"

AST::ASTPtr Parser::assignment() {
    auto curNode = std::make_unique<AST::Assignment>();
    next(); // consume "let"

    // ident
    if (curToken.type != TokenType::IDENT) {
        // syntax error
    }
    curNode->ident = curToken.name;
    next(); // consume ident

    next(); // consume "assignment"

    curNode->append(expression());

    return curNode;
}

AST::ASTPtr Parser::funcCall() {
    auto curNode = std::make_unique<AST::FuncCall>();
    next(); //consume "call"

    // ident
    if (curToken.type != TokenType::IDENT) {
        // syntax error
    }
    curNode->ident = curToken.name;
    next(); // consume ident

    next(); // consume "("

    do {
        if (match(curToken, TokenType::PUNCTUATION, ",")) {
            next(); // consume ","
        }

        curNode->append(expression());
    } while (match(curToken, TokenType::PUNCTUATION, ","));

    next(); // consume ")"

    return curNode;
}

AST::ASTPtr Parser::ifStatement() {
    // unimplemented
    return nullptr;
}

AST::ASTPtr Parser::whileStatement() {
    // unimplemented
    return nullptr;
}

AST::ASTPtr Parser::returnStatement() {
    // unimplemented
    return nullptr;
}

AST::ASTPtr Parser::statement() {
    // assignment | funcCall | ifStatement | whileStatement | returnStatement

    if (match(curToken, TokenType::KEYWORD, "let")) return assignment();
    if (match(curToken, TokenType::KEYWORD, "call")) return funcCall();
    if (match(curToken, TokenType::KEYWORD, "if")) return ifStatement();
    if (match(curToken, TokenType::KEYWORD, "while")) return whileStatement();
    if (match(curToken, TokenType::KEYWORD, "return")) return returnStatement();

    // Syntax error
    return nullptr;
}

AST::ASTPtr Parser::statSequence() {
    // statement {";" statement } [";"] .

    auto curNode = std::make_unique<AST::StatSequence>();

    do {
        if (match(curToken, TokenType::PUNCTUATION, ";")) next(); // consume ";"

        curNode->append(statement());
    } while (match(curToken, TokenType::PUNCTUATION, ";"));

    return curNode;
}
