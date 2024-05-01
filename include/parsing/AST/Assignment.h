#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <string>
#include "AST/Statement.h"

namespace AST {
    class Assignment : public Statement {
    public:
        std::string ident;
        Assignment() = default;

        int evaluate(IRBuilder& builder, int& block) override {
            auto tmp = children[0]->evaluate(builder, block);
            builder.blocks[block].nameTable[ident] = tmp;

            return 0;
        }
    };
}

#endif //ASSIGNMENT_H
