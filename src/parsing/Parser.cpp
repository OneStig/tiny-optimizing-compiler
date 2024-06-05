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

    auto curNode = std::make_unique<AST::FuncDecl>();
    curNode->isVoid = false;
    isVoid = false;

    if (match(curToken, TokenType::KEYWORD, "function")) {
        next(TokenType::KEYWORD); // consume 'function'
        curNode->isVoid = true;
        isVoid = true;
    }

    curNode->name = curToken.name;
    next(TokenType::IDENT); // consume ident

    // formalParam
    next(TokenType::PUNCTUATION); // consume (

    if (!match(curToken, TokenType::PUNCTUATION, ")")) {
        curNode->parameters.push_back(curToken.name);
        next(TokenType::IDENT);

        while (match(curToken, TokenType::PUNCTUATION, ",")) {
            next(TokenType::PUNCTUATION);
            curNode->parameters.push_back(curToken.name);
            next(TokenType::IDENT);
        }
    }

    next(TokenType::PUNCTUATION); // consume )
    next(TokenType::PUNCTUATION); // consume ;

    // funcBody

    if (match(curToken, TokenType::KEYWORD, "var")) {
        curNode->append(varDecl());
    }

    // "{" statSequence "}" "."
    next(TokenType::PUNCTUATION); // Consume '{'
    if (curToken.type == TokenType::KEYWORD &&
        (curToken.name == "let" || curToken.name == "call" || curToken.name == "if" ||
            curToken.name == "while" || curToken.name == "return")) {
        curNode->append(statSequence());
    }
    next(TokenType::PUNCTUATION); // Consume '}'
    next(TokenType::PUNCTUATION); // Consume ;

    isVoid = true;

    return curNode;
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
