#include "lexing/Tiny.h"
#include "parsing/AST/Statements.h"

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
            if (builder.blocks[follow].nameTable[varName] == 0) {
                builder.blocks[follow].nameTable[varName] = builder.emit(0, InsType::CONST, 0);

                if (!builder.analysisMode) {
                    unusedWarning(varName);
                }
            }
            else if (builder.blocks[branch].nameTable[varName] == 0) {
                builder.blocks[branch].nameTable[varName] = builder.emit(0, InsType::CONST, 0);

                if (!builder.analysisMode) {
                    unusedWarning(varName);
                }
            }

            const int varVal = builder.emit(join, InsType::PHI,
                builder.blocks[follow].nameTable[varName],
                builder.blocks[branch].nameTable[varName]);

            builder.blocks[join].nameTable[varName] = varVal;
        }
    }

    block = join;

    return 0;
}
