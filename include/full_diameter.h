/*
 * Full run diamter: compute the diameter by running BFS on all nodes
 */

#pragma once

#include "bfs.h"
#include "graph.h"

namespace full {
template <bool WithRadius = false>
struct Diameter
{
    std::size_t operator()(const Graph& graph)
    {
        min_excentricity = graph.order();

        BFS bfs(graph.order());
        for (std::size_t v = 0; not found() and v < graph.order(); ++v) {
            bfs.reset();
            bfs(graph, v);
            max_excentricity = std::max(max_excentricity, bfs.max_distance);
            min_excentricity = std::min(min_excentricity, bfs.max_distance);
            ++runs;
            connected = bfs.visited_vertices == graph.order();
        }
        return max_excentricity;
    }

    bool found() const
    {
        if constexpr (WithRadius) {
            return max_excentricity == min_excentricity * 2;
        } else {
            return false;
        }
    }

    std::size_t max_excentricity = 0;
    std::size_t min_excentricity = 0;
    std::size_t runs             = 0;
    bool        connected        = true;
};
};
