#include "parsing/AST/Declarations.h"

int AST::Computation::evaluate(IRBuilder &builder, int& block) {
    int computationBlock = builder.newBlock();
    builder.blocks[block].to = computationBlock;

    for (const ASTPtr &child: children) {
        child->evaluate(builder, computationBlock);
    }

    // fix the branch joins
    for (const auto& br : builder.branches) {
        if (builder.blocks[br.second].instructions.empty()) {
            builder.emit(br.second, InsType::MT);
        }

        if (builder.blocks[br.first].instructions.back().type == InsType::BRA) {
            builder.blocks[br.first].instructions.back().x = builder.blocks[br.second].instructions.front().id;
        }
        else {
            builder.blocks[br.first].instructions.back().y = builder.blocks[br.second].instructions.front().id;
        }
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
