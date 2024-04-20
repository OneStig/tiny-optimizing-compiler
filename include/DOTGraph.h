//
// Created by Steven He on 4/19/24.
//

#ifndef DOTGRAPH_H
#define DOTGRAPH_H

#include <vector>
#include <string>

#include "BasicBlock.h"

class DOTGraph {
public:
    explicit DOTGraph(const std::vector<BasicBlock>& blocks);

    std::string visualize();

private:
    std::string construct;
};

#endif //DOTGRAPH_H
