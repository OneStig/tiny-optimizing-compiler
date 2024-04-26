#ifndef BASICBLOCK_H
#define BASICBLOCK_H

#include <deque>
#include <string>
#include <unordered_map>

#include "Instruction.h"

class BasicBlock {
public:
    std::deque<Instruction> instructions;
    std::unordered_map<std::string, int> nameTable;

    int blockNum;
    int follow{-1};
    int branch{-1};

    explicit BasicBlock(const int num) : blockNum{num} {}

    [[nodiscard]] std::string toString() const {
        std::string str = "BB" + std::to_string(blockNum) + "|";

        if (!instructions.empty()) {
            str += "{";
            bool first = true;
            for (const Instruction& i : instructions) {
                if (!first) {
                    str += "|";
                }
                else {
                    first = false;
                }
                str += i.toString();
            }

            str += "}";
        }

        return str;
    }
};

#endif //BASICBLOCK_H
