#include "DOTGraph.h"

DOTGraph::DOTGraph(const std::vector<BasicBlock>& blocks) {
    construct = "digraph G {\n";

    for (int i = 0; i < blocks.size(); i++) {
        construct += "bb" + std::to_string(i);
        construct += " [shape=record, label=\"<b>" + blocks[i].toString() + "\"];\n";
    }

    construct += "}";
}

std::string DOTGraph::visualize() {
    return construct;
}