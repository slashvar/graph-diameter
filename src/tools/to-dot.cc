/*
 * Convert a graph to GraphViz DOT format.
 */

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>

#include "graph.h"
#include "nde.h"

namespace {

template <typename Stream>
void to_dot(const Graph& graph, Stream& stream)
{
    stream << "graph G  {\n";
    for (std::size_t v = 0; v < graph.order(); ++v) {
        for (auto s : graph[v]) {
            if (s > v) {
                stream << "  " << v << " -- " << s << "\n";
            }
        }
    }
    stream << "}\n";
}

};  // namespace

int main(int argc, char* argv[])
try {
    const std::span args(argv, static_cast<std::size_t>(argc));
    if (args.size() < 2) {
        std::cerr << "missing file name\n";
        return EXIT_FAILURE;
    }
    const auto graph = nde::load(args[1]);
    to_dot(graph, std::cout);
    return EXIT_SUCCESS;
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
}
