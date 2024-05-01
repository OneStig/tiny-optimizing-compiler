#ifndef STATEMENT_H
#define STATEMENT_H

#include "ASTNode.h"
#include "Expressions.h"

namespace AST {

class StatSequence : public ASTNode {
public:
    StatSequence() = default;

    int evaluate(IRBuilder &builder, int &block) override;
};

class Statement : public ASTNode {
public:
    Statement() = default;
};

class Assignment : public Statement {
public:
    std::string ident;
    Assignment() = default;

    int evaluate(IRBuilder& builder, int& block) override;
};

class FuncCall : public Statement { // children represent parameters
public:
    std::string ident;
    FuncCall() = default;

    int evaluate(IRBuilder& builder, int& block) override;
};

class IfStatement : public Statement {
public:
    IfStatement() = default;
    std::unique_ptr<Relation> relation;

    int evaluate(IRBuilder &builder, int& block) override;
};

class WhileStatement : public Statement {
public:
    WhileStatement() = default;
    std::unique_ptr<Relation> relation;

    int evaluate(IRBuilder &builder, int& block) override;
};

}

#endif //STATEMENT_H
