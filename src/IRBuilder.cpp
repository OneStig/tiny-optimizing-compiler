#include "IRBuilder.h"

int IRBuilder::emit(const int& block, const InsType type, const int x, const int y) {
    int id = nextInstr();
    blocks[block].instructions.emplace_back(id, type, x, y);
    return id;
}

int IRBuilder::newBlock() {
    int blockID = static_cast<int>(blocks.size());
    blocks.emplace_back(blockID);
    return blockID;
}
