#include "../../include/synthesis/IRBuilder.h"

int IRBuilder::emit(const int& block, const InsType type, const int x, const int y, const bool front) {
    const InstrSig signature{type, x, y};

    // check if value has been computed in cur block, or anywhere up dom tree
    if (type != InsType::READ && type != InsType::PHI && type != InsType::WRITE &&
        type != InsType::MT) {
        int dominated = block;

        while (dominated != -1) {
            if (blocks[dominated].redunInstr.contains(signature)) {
                return blocks[dominated].redunInstr[signature];
            }

            dominated = blocks[dominated].domBy;
        }
    }

    // if instruction is not redundant, create new instruction and put it in table
    int id = nextInstr();
    if (front) {
        blocks[block].instructions.emplace_front(id, type, x, y);
    }
    else {
        blocks[block].instructions.emplace_back(id, type, x, y);
    }
    blocks[block].redunInstr[signature] = id;
    return id;
}

int IRBuilder::newBlock() {
    int blockID = static_cast<int>(blocks.size());
    blocks.emplace_back(blockID);
    return blockID;
}

void IRBuilder::cleanUp() {
    // first populate empty blocks with empty instruction
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].instructions.empty()) {
            emit(i, InsType::MT);
        }
    }

    // then replace branch block values with instruction #
    // branch numbers are stored as negative numbers

    for (BasicBlock& block : blocks) {
        Instruction& last = block.instructions.back();

        if (last.type == InsType::BRA) {
            last.x = blocks[-last.x].instructions.front().id;
        }
        else if (last.type == InsType::BNE || last.type == InsType::BEQ ||
                last.type == InsType::BLE || last.type == InsType::BLT ||
                last.type == InsType::BGE || last.type == InsType::BGT) {
            last.y = blocks[-last.y].instructions.front().id;
        }
    }
}