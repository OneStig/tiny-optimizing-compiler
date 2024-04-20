#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "BasicBlock.h"
#include "Lexer.h"

class Parser {
private:
    const Lexer& lx;
    std::vector<BasicBlock> blocks;
    int curBlock{};

    int factor();
    int term();
    int expression();

    void computation();

public:
    explicit Parser(const Lexer& lexer) : lx{lexer} {
        computation();
    }

    [[nodiscard]] const std::vector<BasicBlock>& getBlocks() const {
        return blocks;
    }
};

#endif //PARSER_H
