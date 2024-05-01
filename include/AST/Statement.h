#ifndef STATEMENT_H
#define STATEMENT_H

#include "ASTNode.h"
#include "Expression.h"

namespace AST {

class Statement : public ASTNode {
public:
    Statement() = default;
};

class IfStatement : public Statement {
public:
    IfStatement() = default;
    std::unique_ptr<Relation> relation;

    int evaluate(IRBuilder &builder, int& block) override {
        int branch = builder.newBlock();
        int follow = builder.newBlock();
        int join = builder.newBlock();

        // set domination
        builder.blocks[branch].domBy = block;
        builder.blocks[follow].domBy = block;
        builder.blocks[join].domBy = block;

        builder.blocks[block].branch = branch;
        builder.blocks[block].follow = follow;

        builder.blocks[branch].follow = join;
        builder.blocks[follow].branch = join;

        // copy branch and follow name tables
        builder.blocks[branch].nameTable = builder.blocks[block].nameTable;
        builder.blocks[follow].nameTable = builder.blocks[block].nameTable;

        const int cmpInstr = relation->evaluate(builder, block); // compares the two things

        // branch statSequence
        children[0]->evaluate(builder, branch);

        // follow statSequence
        children[1]->evaluate(builder, follow);
        builder.emit(follow, InsType::BRA, join);

        // branch instruction from block
        if (builder.blocks[branch].instructions.empty()) {
            builder.emit(branch, InsType::MT);
        }

        builder.emit(block, relation->relType, cmpInstr, builder.blocks[branch].instructions[0].id);

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
};

}

#endif //STATEMENT_H
