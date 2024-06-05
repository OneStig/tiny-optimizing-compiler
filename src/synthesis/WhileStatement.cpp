#include "parsing/AST/Statements.h"

int AST::WhileStatement::evaluate(IRBuilder& builder, int& block) {
    int head = builder.newBlock();
    const int origBody = builder.newBlock();
    int exit = builder.newBlock();
    int body = origBody;

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

    // take a snapshot of state here to roll back to later in (2)
    const unsigned long totalBlocks = builder.blocks.size();
    const int snapshotSSA = builder.instructionNum;

    int cmpInstr = relation->evaluate(builder, head); // compares the two things
    builder.emit(head, relation->relType, cmpInstr, -exit);
    children[0]->evaluate(builder, body);
    builder.emit(body, InsType::BRA, -head);

    // figure out phi
    // head and body may have inconsistencies to resolve

    // 1. figure out which variables in head and body are different

    std::vector<std::string> diffVars;

    for (auto& [var, ssa] : builder.blocks[head].nameTable) {
        if (ssa == 0) continue; // ignore uninitialized variables for now

        if (builder.blocks[body].nameTable[var] != ssa) {
            diffVars.push_back(var);
        }
    }

    // 2. rollback all generated code to the current state.

    body = origBody;

    while (builder.blocks.size() > totalBlocks) {
        builder.blocks.pop_back();
    }

    builder.blocks[head] = BasicBlock(head);
    builder.blocks[body] = BasicBlock(body);
    builder.blocks[exit] = BasicBlock(exit);

    builder.instructionNum = snapshotSSA;

    builder.blocks[block].to = head;

    // set domination
    builder.blocks[head].domBy = block;
    builder.blocks[body].domBy = head;
    builder.blocks[exit].domBy = head;

    // copy name tables
    builder.blocks[head].nameTable = builder.blocks[block].nameTable;

    builder.blocks[head].follow = body;

    // 3. generate necessary phi functions

    for (std::string& var : diffVars) {
        const int phi = builder.emit(head, InsType::PHI, builder.blocks[head].nameTable[var]);
        builder.blocks[head].nameTable[var] = phi;
    }

    builder.blocks[body].nameTable = builder.blocks[head].nameTable;

    // 4. regenerate everything with new values

    cmpInstr = relation->evaluate(builder, head); // compares the two things
    builder.emit(head, relation->relType, cmpInstr, -exit);
    children[0]->evaluate(builder, body);
    builder.emit(body, InsType::BRA, -head);

    // 5. fix right side of the phi values

    auto it = builder.blocks[head].instructions.begin();
    for (std::string& var : diffVars) {
        it->y = builder.blocks[body].nameTable[var];
        ++it;
    }

    // after body is filled
    builder.blocks[exit].nameTable = builder.blocks[head].nameTable;
    builder.blocks[body].branch = head;
    builder.blocks[head].branch = exit;

    block = exit;

    return 0;
}