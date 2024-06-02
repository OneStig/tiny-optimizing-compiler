#include "parsing/AST/Declarations.h"

SSA AST::Computation::evaluate(IRBuilder &builder, int& block) {
    int computationBlock = builder.newBlock();
    builder.blocks[block].to = computationBlock;

    for (const ASTPtr &child: children) {
        child->evaluate(builder, computationBlock);
    }

    return NULL_SSA;
}

SSA AST::VarDecl::evaluate(IRBuilder &builder, int& block) {
    // Do something later to check for undeclared variables
    for (std::string& var : variables) {
        builder.blocks[block].nameTable[var] = 0;
    }
    return NULL_SSA;
}
