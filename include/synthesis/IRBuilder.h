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

    int emit(const int& block, InsType type, int x = 0, int y = 0, bool front = false);
    int newBlock();

    void cleanUp();

private:
    int instructionNum{};

    int nextInstr() {
        return ++instructionNum;
    }
};



#endif //IRBUILDER_H
