#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include "ASTNode.h"

namespace AST {
    class Expression : public ASTNode {
    public:
        Expression() = default;
        std::vector<bool> negate; // starting after first child

        int evaluate(IRBuilder& builder, int& block) override;
    };

    class Term : public ASTNode {
    public:
        Term() = default;
        std::vector<bool> divide; // starting after first child

        int evaluate(IRBuilder& builder, int& block) override;
    };

    class Factor : public ASTNode {
    public:
        Factor() = default;

        int evaluate(IRBuilder& builder, int& block) override;
    };

    class Identifier : public ASTNode {
    public:
        Identifier() = default;
        std::string name;

        int evaluate(IRBuilder& builder, int& block) override;
    };

    class Number : public ASTNode {
    public:
        Number() = default;
        int value{};

        int evaluate(IRBuilder &builder, int& block) override;
    };

    class Relation : public ASTNode {
    public:
        Relation() = default;
        InsType relType;

        int evaluate(IRBuilder &builder, int& block) override;
    };
}

#endif //EXPRESSION_H
