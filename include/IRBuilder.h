#ifndef IRBUILDER_H
#define IRBUILDER_H

#include <vector>

#include "BasicBlock.h"

class IRBuilder {
public:
    std::vector<BasicBlock> blocks;
    std::vector<std::pair<int, int>> branches;

    IRBuilder() {
        // Constants block
        newBlock();
    }

    int emit(const int& block, InsType type, int x = 0, int y = 0);
    int newBlock();

private:
    int instructionNum{};

    int nextInstr() {
        return ++instructionNum;
    }
};



#endif //IRBUILDER_H
