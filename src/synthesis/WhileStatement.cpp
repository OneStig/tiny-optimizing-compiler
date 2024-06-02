#include "parsing/AST/Statements.h"

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

    std::unordered_map<int, int> phiTable;

    for (auto& [var, ssa] : builder.blocks[head].nameTable) {
        if (ssa == 0) continue;

        if (builder.blocks[body].nameTable[var] != ssa) {

        }
    }

    // 2. replace everything with new values


    // after body is filled
    builder.blocks[exit].nameTable = builder.blocks[head].nameTable;
    builder.blocks[body].branch = head;
    builder.blocks[head].branch = exit;

    block = exit;

    return 0;
}