#ifndef IRBUILDER_H
#define IRBUILDER_H

#include <vector>

#include "BasicBlock.h"

class IRBuilder {
public:
    std::vector<BasicBlock> blocks;

    IRBuilder() {
        // Constants block
        newBlock();
    }

    SSA emit(const int& block, InsType type, SSA x = NULL_SSA, SSA y = NULL_SSA, bool front = false);
    int newBlock();

    void cleanUp();

private:
    int instructionNum{};

    int nextInstr() {
        return ++instructionNum;
    }
};



#endif //IRBUILDER_H
