#include "parsing/Parser.h"
#include "parsing/AST/Statements.h"

std::unique_ptr<AST::Assignment> Parser::assignment() {
    auto curNode = std::make_unique<AST::Assignment>();
    next(TokenType::KEYWORD); // consume "let"

    // ident
    if (curToken.type != TokenType::IDENT) {
        // syntax error
    }
    curNode->ident = curToken.name;
    next(TokenType::IDENT); // consume ident

    next(TokenType::ASSIGN); // consume "assignment"

    curNode->append(expression());

    return curNode;
}

AST::ASTPtr Parser::funcCall() {
    auto curNode = std::make_unique<AST::FuncCall>();
    next(TokenType::KEYWORD); //consume "call"

    // ident
    if (curToken.type != TokenType::IDENT) {
        // syntax error
    }
    curNode->ident = curToken.name;
    next(TokenType::IDENT); // consume ident

    if (match(curToken, TokenType::PUNCTUATION, "(")) {
        next(TokenType::PUNCTUATION); // consume "("

        if (!match(curToken, TokenType::PUNCTUATION, ")")) {
            do {
                if (match(curToken, TokenType::PUNCTUATION, ",")) {
                    next(TokenType::PUNCTUATION); // consume ","
                }

                curNode->append(expression());
            } while (match(curToken, TokenType::PUNCTUATION, ","));
        }

        next(TokenType::PUNCTUATION); // consume ")"
    }

    return curNode;
}

AST::ASTPtr Parser::ifStatement() {
    // first child is then statSequence
    // [optional] second child is else statSequence

    auto curNode = std::make_unique<AST::IfStatement>();
    next(TokenType::KEYWORD); // consusme "if"
    curNode->relation = std::move(relation());
    next(TokenType::KEYWORD); // consume "then"
    curNode->append(statSequence());

    if (match(curToken, TokenType::KEYWORD, "else")) {
        next(TokenType::KEYWORD); // consume 'else'
        curNode->append(statSequence());
    }

    next(TokenType::KEYWORD); // consume 'fi'

    return curNode;
}

AST::ASTPtr Parser::whileStatement() {
    auto curNode = std::make_unique<AST::WhileStatement>();
    next(TokenType::KEYWORD); // consume "while"
    curNode->relation = std::move(relation());
    next(TokenType::KEYWORD); // consume "do"
    curNode->append(statSequence());
    next(TokenType::KEYWORD); // consume "od"

    return curNode;
}

AST::ASTPtr Parser::returnStatement() {
    auto curNode = std::make_unique<AST::ReturnStatement>();
    next(TokenType::KEYWORD); // consume "return"

    if (!isVoid) {
        curNode->append(expression());
    }

    return curNode;
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

    curNode->append(statement());

    while (match(curToken, TokenType::PUNCTUATION, ";")) {
        next(TokenType::PUNCTUATION); // consume ";"
        AST::ASTPtr isState = statement();

        if (isState == nullptr) break;

        curNode->append(std::move(isState));
    }

    return curNode;
}
