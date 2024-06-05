#include "../../include/synthesis/IRBuilder.h"

#include <stack>

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
    int id;

    if (type == InsType::CONST) {
        id = nextConst();
    }
    else {
        id = nextInstr();
    }

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

    // then let's go through and re-number all instructions
    // int nextID{1};
    // std::stack<int> toRenum;
    // std::unordered_map<int, int> renumberings;
    // toRenum.push(0);
    //
    // auto renum = [&renumberings, &nextID](int& id) -> void {
    //     if (id == 0) return;
    //
    //     if (!renumberings.contains(id)) {
    //         renumberings[id] = nextID++;
    //     }
    //
    //     id = renumberings[id];
    // };
    //
    // while (!toRenum.empty()) {
    //     const int block = toRenum.top();
    //     toRenum.pop();
    //
    //     if (blocks[block].branch != -1) {
    //         toRenum.push(blocks[block].branch);
    //     }
    //
    //     if (blocks[block].follow != -1) {
    //         toRenum.push(blocks[block].follow);
    //     }
    //
    //     if (blocks[block].to != -1) {
    //         toRenum.push(blocks[block].to);
    //     }
    //
    //     for (Instruction& ins : blocks[block].instructions) {
    //         renum(ins.id);
    //     }
    // }
    //
    // // second renumbering pass
    //
    // for (BasicBlock& block : blocks) {
    //     for (Instruction& ins : block.instructions) {
    //         if (ins.type != InsType::MT && ins.type != InsType::CONST &&
    //             ins.type != InsType::READ && ins.type != InsType::WRITENL) {
    //             renum(ins.x);
    //             renum(ins.y);
    //         }
    //     }
    // }
}