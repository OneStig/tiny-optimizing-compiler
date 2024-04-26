#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <string>
#include "AST/Statement.h"

namespace AST {
    class Assignment : public Statement {
    public:
        std::string ident;
        Assignment() = default;

        int evaluate(IRBuilder &builder, int block) override {
            builder.blocks[block].nameTable[ident] = children[0]->evaluate(builder, block);
        }
    };
}

#endif //ASSIGNMENT_H
