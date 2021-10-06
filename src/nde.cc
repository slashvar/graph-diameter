/*
 * Node Degree Edge
 */

#include <cerrno>
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
        throw fs::filesystem_error(
            "graph file open failed", filename, std::error_code(errno, std::generic_category()));
    }
    // read graph order
    std::size_t order;
    if (not(input >> order)) {
        throw std::invalid_argument("file format error");
    }
    Graph graph(order);
    // Read degree
    for (std::size_t i = 0; i < order; ++i) {
        std::size_t v, d;
        if (input >> v >> d) {
            graph.reserve_edges(v, d);
        } else {
            throw std::invalid_argument("unexpected end of input");
        }
    }
    // Read edges
    std::size_t v0 {};
    std::size_t v1 {};
    while (input >> v0 >> v1) {
        graph.add_edge(v0, v1);
    }
    graph.sort_successors();
    return graph;
}

};
