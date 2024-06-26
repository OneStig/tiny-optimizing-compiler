#ifndef BASICBLOCK_H
#define BASICBLOCK_H

#include <list>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "Instruction.h"

class BasicBlock {
public:
    std::list<Instruction> instructions;
    std::unordered_map<std::string, int> nameTable;

    std::unordered_map<InstrSig, int> redunInstr;
    int domBy{-1}; // store dominator tree digraph in reverse

    int blockNum;
    int follow{-1};
    int branch{-1};
    int to{-1};

    std::unordered_map<std::string, std::unordered_map<int, std::vector<int>>> useChain;

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
