#ifndef COMPUTATION_H
#define COMPUTATION_H

#include <string>
#include "ASTNode.h"

namespace AST {

class Computation : public ASTNode {
public:
    Computation() = default;

    int evaluate(IRBuilder &builder, int& block) override {
        int computationBlock = builder.newBlock();
        builder.blocks[block].to = computationBlock;

        for (const ASTPtr &child: children) {
            child->evaluate(builder, computationBlock);
        }

        // fix the branch joins
        for (const auto& br : builder.branches) {
            builder.blocks[br.first].instructions.back().x = builder.blocks[br.second].instructions.front().id;
        }

        return 0;
    }
};

class VarDecl : public ASTNode {
public:
    VarDecl() = default;
    std::vector<std::string> variables;

    int evaluate(IRBuilder &builder, int& block) override {
        // Do something later to check for undeclared variables
        for (std::string& var : variables) {
            builder.blocks[block].nameTable[var] = 0;
        }
        return 0;
    }
};

} // AST

#endif //COMPUTATION_H
