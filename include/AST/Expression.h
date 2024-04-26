#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include "ASTNode.h"

namespace AST {
    class Expression : public ASTNode {
    public:
        Expression() = default;
        std::vector<bool> negate; // starting after first child

        int evaluate(IRBuilder& builder, int block) override {
            int curSSA = children[0]->evaluate(builder, block);

            for (int i = 1; i < children.size(); i++) {
                int nextSSA = children[i]->evaluate(builder, block);
                InsType type = negate[i - 1] ? InsType::SUB : InsType::ADD;
                curSSA = builder.emit(block, type, curSSA, nextSSA);
            }

            return curSSA;
        }
    };

    class Term : public ASTNode {
    public:
        Term() = default;
        std::vector<bool> divide; // starting after first child

        int evaluate(IRBuilder& builder, int block) override {
            int curSSA = children[0]->evaluate(builder, block);

            for (int i = 1; i < children.size(); i++) {
                int nextSSA = children[i]->evaluate(builder, block);
                InsType type = divide[i - 1] ? InsType::DIV : InsType::MUL;
                curSSA = builder.emit(block, type, curSSA, nextSSA);
            }

            return curSSA;
        }
    };

    class Factor : public ASTNode {
    public:
        Factor() = default;

        int evaluate(IRBuilder& builder, int block) override {
            // return ssa value of 1st child regardless of num, ident, expr
            return children[0]->evaluate(builder, block);
        }
    };

    class Identifier : public ASTNode {
    public:
        Identifier() = default;
        std::string name;

        int evaluate(IRBuilder& builder, int block) override {
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

    class Relation : public ASTNode {
    public:
        Relation() = default;
        InsType relType;

        int evaluate(IRBuilder &builder, int block) override {
            return builder.emit(block, InsType::CMP,
                children[0]->evaluate(builder, block),
                children[1]->evaluate(builder, block));
        }
    };
}

#endif //EXPRESSION_H
