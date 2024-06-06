#include "parsing/AST/Statements.h"

int AST::Assignment::evaluate(IRBuilder& builder, int& block) {
    const int tmp = children[0]->evaluate(builder, block);
    builder.blocks[block].nameTable[ident] = tmp;

    return 0;
}

int AST::FuncCall::evaluate(IRBuilder& builder, int& block)  {
    // Predefined functions
    if (ident == "InputNum") return builder.emit(block, InsType::READ);
    if (ident == "OutputNum") return builder.emit(block, InsType::WRITE,
                                children[0]->evaluate(builder, block));
    if (ident == "OutputNewLine") return builder.emit(block, InsType::WRITENL);

    // TODO: Add custom user functions
    if (builder.functionMap.contains(ident)) {
        std::vector<int> params(children.size());

        for (int i = 0; i < children.size(); i++) {
            params[i] = children[i]->evaluate(builder, block);
        }

        for (int i = 0; i < params.size(); i++) {
            builder.emit(block, InsType::SETPAR, i + 1, params[i]);
        }

        return builder.emit(block, InsType::JSR, -builder.functionMap[ident]);
    }

    return 0;
}

int AST::StatSequence::evaluate(IRBuilder &builder, int &block) {
    for (const ASTPtr &child: children) {
        child->evaluate(builder, block);
    }

    return 0;
}

int AST::ReturnStatement::evaluate(IRBuilder &builder, int &block) {
    if (children.empty()) {
        builder.emit(block, InsType::RET);
    }
    else {
        builder.emit(block, InsType::RET, children[0]->evaluate(builder, block));
    }
    return 0;
}