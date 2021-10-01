/*
 * Node Degree Edge
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "graph.h"

namespace fs = std::filesystem;

namespace nde {

Graph load(fs::path filename)
{
    std::ifstream input(filename);
    if (not input) {
        return {};
    }
    // read graph order
    std::size_t order;
    input >> order;
    Graph graph(order);
    // Read degree
    for (std::size_t i = 0; i < order; ++i) {
        std::size_t v, d;
        input >> v >> d;
        graph.reserve_edges(v, d);
    }
    // Read edges
    std::size_t v0 {};
    std::size_t v1 {};
    while (input >> v0 >> v1) {
        graph.add_edge(v0, v1);
    }
    return graph;
}

};
