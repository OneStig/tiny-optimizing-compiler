#ifndef STATSEQUENCE_H
#define STATSEQUENCE_H

#include "ASTNode.h"

namespace AST {
    class StatSequence : public ASTNode {
    public:
        StatSequence() = default;

        int evaluate(IRBuilder &builder, int &block) override {
            int curBlock = block;

            for (const ASTPtr &child: children) {
                child->evaluate(builder, curBlock);
            }

            return 0;
        }
    };
}

#endif //STATSEQUENCE_H
