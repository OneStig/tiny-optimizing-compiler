#include "graph/DOTGraph.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

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
                        + std::to_string(blocks[i].branch) + ":n [constraint=false, color=red, label=\"branch\"];\n";
        }

        // domination
        if (blocks[i].domBy != -1) {
            domination += "\tbb" + std::to_string(blocks[i].domBy) +
                ":b -> bb" + std::to_string(i) +
                ":b [color=blue, style=dotted, label=\"dom\"]\n";
        }
    }

    construct += controlFlow + domination + "}";

    // add comments for
}

std::string urlEncode(const std::string &data) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : data) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
        escaped << std::nouppercase;
    }

    return escaped.str();
}

void DOTGraph::visualize() const {
    std::cout << "https://dreampuf.github.io/GraphvizOnline/#" << urlEncode(construct) << std::endl;
}