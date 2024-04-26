#ifndef COMPUTATION_H
#define COMPUTATION_H

#include "ASTNode.h"

namespace AST {

class Computation : public ASTNode {
public:
    Computation() = default;

    int evaluate(IRBuilder &builder, int block) override {
        const int computationBlock = builder.newBlock();

        for (const ASTPtr &child: children) {
            child->evaluate(builder, computationBlock);
        }

        return 0;
    }
};

} // AST

#endif //COMPUTATION_H
