#include "graph/DOTGraph.h"

#include <gvc.h>
#include <cgraph.h>

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
        if (blocks[i].domBy != -1) {
            domination += "\tbb" + std::to_string(blocks[i].domBy) +
                ":b -> bb" + std::to_string(i) +
                ":b [color=blue, style=dotted, label=\"dom\"]\n";
        }
    }

    construct += controlFlow + domination + "}";
}

void DOTGraph::visualize() const {
    GVC_t *gvc = gvContext();
    const char* dotGraph = construct.c_str();

    Agraph_t *graph = agmemread(dotGraph);

    gvLayout(gvc, graph, "dot");
    gvRenderFilename(gvc, graph, "png", "output.png");
    gvFreeLayout(gvc, graph);
    agclose(graph);
    gvFreeContext(gvc);

    std::string command = "open output.png";
    system(command.c_str());
}