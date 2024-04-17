#ifndef BASICBLOCK_H
#define BASICBLOCK_H

#include <deque>
#include <string>
#include "Instruction.h"

class BasicBlock {
public:
    std::deque<Instruction> instructions;
    int blockNum;
    int left{};
    int right{};

    explicit BasicBlock(const int num) : blockNum{num} {}

    [[nodiscard]] std::string toString() const {
        std::string str = "BB" + blockNum;

        if (!instructions.empty()) {
            str += "| {";
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
