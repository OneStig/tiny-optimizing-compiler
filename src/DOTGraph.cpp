#include "DOTGraph.h"

DOTGraph::DOTGraph(const std::vector<BasicBlock>& blocks) {
    construct = "digraph G {\n";
    std::string controlFlow;
    std::string domination;

    for (int i = 0; i < blocks.size(); i++) {
        construct += "\tbb" + std::to_string(i);
        construct += " [shape=record, label=\"<b>" + blocks[i].toString() + "\"];\n";

        // control flow
        if (blocks[i].to != -1) {
            controlFlow += "\tbb" + std::to_string(i) + ":s -> bb"
                        + std::to_string(blocks[i].to) + ":n ;\n";
        }

        if (blocks[i].follow != -1) {
            controlFlow += "\tbb" + std::to_string(i) + ":s -> bb"
                        + std::to_string(blocks[i].follow) + ":n [label=\"fall-through\"];\n";
        }

        if (blocks[i].branch != -1) {
            controlFlow += "\tbb" + std::to_string(i) + ":s -> bb"
                        + std::to_string(blocks[i].branch) + ":n [label=\"branch\"];\n";
        }
        // domination
    }

    construct += controlFlow + domination + "}";
}

std::string DOTGraph::visualize() {
    return construct;
}