#include "parsing/AST/Declarations.h"

int AST::Computation::evaluate(IRBuilder &builder, int& block) {
    int computationBlock = builder.newBlock();
    builder.blocks[block].to = computationBlock;

    builder.isVoid = true;

    for (const ASTPtr &child: children) {
        child->evaluate(builder, computationBlock);
    }

    return 0;
}

int AST::VarDecl::evaluate(IRBuilder &builder, int& block) {
    // Do something later to check for undeclared variables
    for (std::string& var : variables) {
        builder.blocks[block].nameTable[var] = 0;
    }
    return 0;
}

int AST::FuncDecl::evaluate(IRBuilder &builder, int& block) {
    int functionBlock = builder.newBlock();

    builder.isVoid = isVoid;
    builder.functionMap[name] = functionBlock;

    for (const ASTPtr &child: children) {
        child->evaluate(builder, functionBlock);
    }

    builder.isVoid = true;

    return 0;
}
