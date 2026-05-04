/*
 * Node Degree Edge format
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <filesystem>
#include <iosfwd>

#include "graph.h"

namespace fs = std::filesystem;

namespace nde {

template <typename T>
concept output_stream = requires(T& t, std::size_t v, const char* s) {
    t << v;
    t << s;
};

[[nodiscard]] Graph load(std::istream& input);
[[nodiscard]] Graph load(const fs::path& filename);

template <output_stream Stream>
void serialize(const Graph& graph, Stream& out)
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

};  // namespace nde
