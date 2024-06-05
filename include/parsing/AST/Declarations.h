#ifndef COMPUTATION_H
#define COMPUTATION_H

#include <string>
#include "ASTNode.h"

namespace AST {

class Computation : public ASTNode {
public:
    Computation() = default;

    int evaluate(IRBuilder &builder, int& block) override;
};

class VarDecl : public ASTNode {
public:
    VarDecl() = default;
    std::vector<std::string> variables;

    int evaluate(IRBuilder &builder, int& block) override;
};

class FuncDecl : public ASTNode {
public:
    FuncDecl() = default;
    bool isVoid;
    std::string name;
    std::vector<std::string> parameters;

    int evaluate(IRBuilder &builder, int& block) override;
};
} // AST

#endif //COMPUTATION_H
