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

    int evaluate(IRBuilder &builder, int block) override {
        const int branch = builder.newBlock();
        const int follow = builder.newBlock();
        const int join = builder.newBlock();

        builder.blocks[block].branch = branch;
        builder.blocks[block].follow = follow;

        builder.blocks[branch].follow = join;
        builder.blocks[follow].branch = join;

        // copy all name tables
        builder.blocks[branch].nameTable = builder.blocks[block].nameTable;
        builder.blocks[follow].nameTable = builder.blocks[block].nameTable;
        builder.blocks[join].nameTable = builder.blocks[block].nameTable;

        const int cmpInstr = relation->evaluate(builder, block); // compares the two things
        builder.emit(block, relation->relType, cmpInstr);

        // branch statSequence
        children[0]->evaluate(builder, branch);

        // follow statSequence
        children[1]->evaluate(builder, branch);
        builder.emit(follow, InsType::BRA, join);
    }
};

}

#endif //STATEMENT_H
