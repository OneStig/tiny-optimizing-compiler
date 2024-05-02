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

    // negate all values to come back around and fix phi

    std::unordered_map<int, std::string> reverseTable;

    for (const auto& entry : builder.blocks[head].nameTable) {
        int negated = entry.second < 0 ? entry.second : -entry.second;
        builder.blocks[head].nameTable[entry.first] = negated;
        builder.blocks[body].nameTable[entry.first] = negated;

        if (entry.second != negated) {
            reverseTable[negated] = entry.first;
        }
    }

    const int cmpInstr = relation->evaluate(builder, block); // compares the two things
    children[0]->evaluate(builder, body);

    builder.emit(head, relation->relType, cmpInstr, builder.blocks[body].instructions.front().id);
    builder.emit(body, InsType::BRA, head);
    builder.branches.emplace_back(body, head);

    // figure out phi

    std::unordered_map<int, int> instrSwap;

    for (const auto& entry1 : builder.blocks[block].nameTable) {
        const std::string& var = entry1.first;
        int& val2 = builder.blocks[body].nameTable[var];

        // if body is negative, must have been replaced above. if neq, must be positive
        if (entry1.second != builder.blocks[body].nameTable[var]) {
            if (entry1.second == -val2) {
                val2 = -val2;
                builder.blocks[head].nameTable[var] = val2;

                instrSwap[-val2] = val2;
            }
            else {
                instrSwap[-entry1.second] = builder.emit(head, InsType::PHI, entry1.second, val2, true);
            }
        }
    }

    // go through and swap all negatives with instrSwap

    std::unordered_set<int> visited;
    std::queue<int> toReplace;
    toReplace.push(head);
    visited.insert(head);

    while (!toReplace.empty()) {
        const int curReplace = toReplace.front();
        toReplace.pop();

        if (curReplace == exit) {
            continue;
        }

        for (auto& instr : builder.blocks[curReplace].instructions) {
            if (instrSwap.contains(instr.x)) {
                instr.x = instrSwap[instr.x];
            }

            if (instrSwap.contains(instr.y)) {
                instr.y = instrSwap[instr.y];
            }
        }

        if (!visited.contains(builder.blocks[curReplace].to)) {
            visited.insert(builder.blocks[curReplace].to);
            toReplace.push(builder.blocks[curReplace].to);
        }

        if (!visited.contains(builder.blocks[curReplace].to)) {
            visited.insert(builder.blocks[curReplace].to);
            toReplace.push(builder.blocks[curReplace].to);
        }

        if (!visited.contains(builder.blocks[curReplace].to)) {
            visited.insert(builder.blocks[curReplace].to);
            toReplace.push(builder.blocks[curReplace].to);
        }
    }

    // after body is filled
    builder.emit(body, InsType::BRA, builder.blocks[head].instructions.front().id);
    builder.blocks[exit].nameTable = builder.blocks[head].nameTable;

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
