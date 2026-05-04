/*
 * Full-run diameter: runs BFS from every vertex and tracks both the
 * diameter (max eccentricity) and, optionally, the radius (min
 * eccentricity).
 */

#pragma once

#include <algorithm>
#include <cstddef>

#include "bfs.h"
#include "graph.h"

namespace full {

struct Result
{
    std::size_t diameter;
    std::size_t radius;
    std::size_t bfs_runs;
    bool        connected;
};

template <bool WithRadius = false>
[[nodiscard]] Result run(const Graph& graph)
{
    const auto order = graph.order();
    if (order == 0) {
        return { .diameter = 0, .radius = 0, .bfs_runs = 0, .connected = true };
    }

    std::size_t max_ecc   = 0;
    std::size_t min_ecc   = order;
    std::size_t bfs_runs  = 0;
    bool        connected = true;

    BFS  bfs(order);
    auto found = [&] {
        if constexpr (WithRadius) {
            return max_ecc == min_ecc * 2;
        } else {
            return false;
        }
    };

    for (std::size_t v = 0; not found() and v < order; ++v) {
        bfs.reset();
        bfs(graph, v);
        max_ecc   = std::max(max_ecc, bfs.max_distance());
        min_ecc   = std::min(min_ecc, bfs.max_distance());
        connected = connected and bfs.visited() == order;
        ++bfs_runs;
    }

    return { .diameter = max_ecc, .radius = min_ecc, .bfs_runs = bfs_runs, .connected = connected };
}

};  // namespace full
