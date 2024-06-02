#ifndef COMPUTATION_H
#define COMPUTATION_H

#include <string>
#include "ASTNode.h"

namespace AST {

class Computation : public ASTNode {
public:
    Computation() = default;

    SSA evaluate(IRBuilder &builder, int& block) override;
};

class VarDecl : public ASTNode {
public:
    VarDecl() = default;
    std::vector<std::string> variables;

    SSA evaluate(IRBuilder &builder, int& block) override;
};

} // AST

#endif //COMPUTATION_H
