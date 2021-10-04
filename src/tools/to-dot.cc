/*
 * convert to do format
 */

#include <err.h>
#include <filesystem>
#include <iostream>

#include "graph.h"
#include "nde.h"

template<typename Stream>
void to_dot(const Graph& graph, Stream& stream)
{
    stream << "graph G  {\n";

    for (std::size_t v = 0; v < graph.order(); ++v) {
        for (auto&& s : graph[v]) {
            if (s > v) {
                stream << "  " << v << " -- " << s << "\n";
            }
        }
    }

    stream << "}\n";
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        errx(1, "missing file name");
    }

    auto graph = nde::load(argv[1]);

    to_dot(graph, std::cout);
}
