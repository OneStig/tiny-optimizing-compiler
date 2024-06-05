#ifndef IRBUILDER_H
#define IRBUILDER_H

#include <vector>
#include <climits>

#include "BasicBlock.h"

class IRBuilder {
public:
    std::vector<BasicBlock> blocks;
    std::unordered_map<std::string, int> functionMap;

    int instructionNum{};
    int constantNum{INT_MAX};
    bool isVoid;

    IRBuilder() {
        // Constants block
        newBlock();
    }

    int emit(const int& block, InsType type, int x = 0, int y = 0, bool front = false);
    int newBlock();

    void cleanUp();

private:
    int nextInstr() {
        return ++instructionNum;
    }

    int nextConst() {
        return --constantNum;
    }
};



#endif //IRBUILDER_H
