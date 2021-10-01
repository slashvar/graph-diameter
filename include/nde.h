/*
 * Node Degree Edge format
 */

#pragma once

#include <filesystem>

#include "graph.h"

namespace fs = std::filesystem;

namespace nde {

Graph load(fs::path filename);

template <typename Stream>
void serialize(const Graph& graph, Stream&& out)
{
    out << graph.order() << "\n";
    for (std::size_t v = 0; v < graph.order(); ++v) {
        out << v << " " << graph[v].size() << "\n";
    }
    for (std::size_t v = 0; v < graph.order(); ++v) {
        for (auto&& s : graph[v]) {
            if (s > v) {
                out << v << " " << s << "\n";
            }
        }
    }
}
};
