#include "parsing/Parser.h"
#include "parsing/AST/Statements.h"

std::unique_ptr<AST::Assignment> Parser::assignment() {
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
    // first child is then statSequence
    // [optional] second child is else statSequence

    auto curNode = std::make_unique<AST::IfStatement>();
    next(); // consusme "if"
    curNode->relation = std::move(relation());
    next(); // consume "then"
    curNode->append(statSequence());

    if (match(curToken, TokenType::KEYWORD, "else")) {
        next(); // consume 'else'
        curNode->append(statSequence());
    }

    next(); // consume 'fi'

    return curNode;
}

AST::ASTPtr Parser::whileStatement() {
    auto curNode = std::make_unique<AST::WhileStatement>();
    next(); // consume "while"
    curNode->relation = std::move(relation());
    next(); // consume "do"
    curNode->append(statSequence());
    next(); // consume "od"

    return curNode;
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
