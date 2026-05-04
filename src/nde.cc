/*
 * Node Degree Edge
 */

#include "nde.h"

#include <cerrno>
#include <filesystem>
#include <fstream>
#include <istream>
#include <stdexcept>
#include <system_error>

#include "graph.h"

namespace fs = std::filesystem;

namespace nde {

Graph load(std::istream& input)
{
    std::size_t order = 0;
    if (not(input >> order)) {
        throw std::invalid_argument("file format error");
    }
    Graph graph(order);
    for (std::size_t i = 0; i < order; ++i) {
        std::size_t v = 0;
        std::size_t d = 0;
        if (not(input >> v >> d)) {
            throw std::invalid_argument("unexpected end of input");
        }
        if (v >= order) {
            throw std::invalid_argument("vertex id out of range in degree section");
        }
        graph.reserve_edges(v, d);
    }
    std::size_t v0 = 0;
    std::size_t v1 = 0;
    while (input >> v0 >> v1) {
        graph.add_edge(v0, v1);
    }
    graph.sort_successors();
    return graph;
}

Graph load(const fs::path& filename)
{
    std::ifstream input(filename);
    if (not input) {
        throw fs::filesystem_error(
            "graph file open failed", filename, std::error_code(errno, std::generic_category()));
    }
    return load(input);
}

};  // namespace nde
