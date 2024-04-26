#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include "ASTNode.h"

namespace AST {
    class Expression : public ASTNode {
    public:
        Expression() = default;
        std::vector<bool> negate; // starting after first child
    };

    class Term : public ASTNode {
    public:
        Term() = default;
        std::vector<bool> divide; // starting after first child
    };

    class Factor : public ASTNode {
    public:
        Factor() = default;
    };

    class Identifier : public ASTNode {
    public:
        Identifier() = default;
        std::string name;

        int evaluate(IRBuilder &builder, int block) override {
            return builder.blocks[block].nameTable[name];
        }
    };

    class Number : public ASTNode {
    public:
        Number() = default;
        int value{};

        int evaluate(IRBuilder &builder, int block) override {
            return builder.emit(0, InsType::CONST, value);
        }
    };
}

#endif //EXPRESSION_H
