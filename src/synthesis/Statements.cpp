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
    int head = builder.newBlock();
    int body = builder.newBlock();
    int exit = builder.newBlock();

    builder.blocks[block].to = head;
    // condition branches to exit, follows to body
    // body branches back to condition

    // set domination
    builder.blocks[head].domBy = block;
    builder.blocks[body].domBy = head;
    builder.blocks[exit].domBy = head;

    // copy name tables
    builder.blocks[head].nameTable = builder.blocks[block].nameTable;
    builder.blocks[body].nameTable = builder.blocks[block].nameTable;

    builder.blocks[head].follow = body;

    const int cmpInstr = relation->evaluate(builder, head); // compares the two things
    builder.emit(head, relation->relType, cmpInstr, -exit);
    children[0]->evaluate(builder, body);
    builder.emit(body, InsType::BRA, -head);

    // figure out phi
    // head and body may have inconsistencies to resolve

    // 1. figure out which variables in head and body are different

    // 2. replace everything with new values


    // after body is filled
    builder.blocks[exit].nameTable = builder.blocks[head].nameTable;
    builder.blocks[body].branch = head;
    builder.blocks[head].branch = exit;

    block = exit;

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

    int branchCopy = branch;
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

    builder.emit(follow, InsType::BRA, -join);

    // branch instruction from block
    builder.emit(block, relation->relType, cmpInstr, -branchCopy);

    builder.blocks[branch].follow = join;
    builder.blocks[follow].branch = join;

    // apply phi values

    for (const std::string& varName : std::views::keys(builder.blocks[block].nameTable)) {
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
