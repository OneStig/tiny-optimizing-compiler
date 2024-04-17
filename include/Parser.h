#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "BasicBlock.h"

class Parser {
private:
    std::vector<BasicBlock> blocks;

    int factor();
    int term();
    int expression();
};

#endif //PARSER_H
