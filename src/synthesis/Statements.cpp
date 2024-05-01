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

    return 0;
}

int AST::WhileStatement::evaluate(IRBuilder& builder, int& block) {
    return 0;
}

int AST::IfStatement::evaluate(IRBuilder &builder, int& block) {
    int branch = builder.newBlock();
    int follow = builder.newBlock();
    int join = builder.newBlock();

    // set domination
    builder.blocks[branch].domBy = block;
    builder.blocks[follow].domBy = block;
    builder.blocks[join].domBy = block;

    builder.blocks[block].branch = branch;
    builder.blocks[block].follow = follow;


    // copy branch and follow name tables
    builder.blocks[branch].nameTable = builder.blocks[block].nameTable;
    builder.blocks[follow].nameTable = builder.blocks[block].nameTable;

    const int cmpInstr = relation->evaluate(builder, block); // compares the two things

    // follow statSequence (if condition)
    children[0]->evaluate(builder, follow);

    // branch statSequence (else)
    if (children.size() == 2) {
        children[1]->evaluate(builder, branch);
    }

    builder.emit(follow, InsType::BRA, join);
    builder.branches.emplace_back(follow, join);

    // branch instruction from block
    if (builder.blocks[branch].instructions.empty()) {
        builder.emit(branch, InsType::MT);
    }

    builder.emit(block, relation->relType, cmpInstr, builder.blocks[branch].instructions[0].id);

    builder.blocks[branch].follow = join;
    builder.blocks[follow].branch = join;

    // apply phi values

    for (const auto& entry : builder.blocks[block].nameTable) {
        const std::string& varName = entry.first;

        if (builder.blocks[branch].nameTable[varName] ==
            builder.blocks[follow].nameTable[varName]) {
            builder.blocks[join].nameTable[varName] = builder.blocks[branch].nameTable[varName];
        }
        else {
            const int varVal = builder.emit(join, InsType::PHI,
                builder.blocks[follow].nameTable[varName],
                builder.blocks[branch].nameTable[varName]);

            builder.blocks[join].nameTable[varName] = varVal;
        }
    }

    block = join;

    return 0;
}

int AST::StatSequence::evaluate(IRBuilder &builder, int &block) {
    for (const ASTPtr &child: children) {
        child->evaluate(builder, block);
    }

    return 0;
}
