#include "IRBuilder.h"

int IRBuilder::emit(const int& block, const InsType type, const int x, const int y) {
    const InstrSig signature{type, x, y};

    // check if value has been computed in cur block, or anywhere up dom tree
    if (type != InsType::READ) {
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
    blocks[block].instructions.emplace_back(id, type, x, y);
    blocks[block].redunInstr[signature] = id;
    return id;
}

int IRBuilder::newBlock() {
    int blockID = static_cast<int>(blocks.size());
    blocks.emplace_back(blockID);
    return blockID;
}
