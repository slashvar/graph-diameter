/*
 * smart diamter: compute the diameter trying to skip useless nodes
 */

#pragma once

#include <deque>
#include <vector>

#include "bfs.h"
#include "graph.h"

namespace smart {

struct Diameter
{

    std::size_t operator()(const Graph& graph)
    {
        candidate.push_back(0);
        while (not candidate.empty() and not found()) {
            auto v = candidate.front();
            candidate.pop_front();
            if (done[v]) {
                continue;
            }
            done[v] = true;
            bfs.reset();
            bfs(graph, v);
            max_excentricity = std::max(max_excentricity, bfs.max_distance);
            min_excentricity = std::min(min_excentricity, bfs.max_distance);
            ++runs;
            if (not done[bfs.last]) {
                v       = bfs.last;
                done[v] = true;
                bfs.reset();
                bfs(graph, v);
                max_excentricity = std::max(max_excentricity, bfs.max_distance);
                min_excentricity = std::min(min_excentricity, bfs.max_distance);
                ++runs;
            }
            populate();
        }
        return max_excentricity;
    }

    void populate()
    {
        for (std::size_t v = 0; v < order; ++v) {
            if (not done[v] and not scheduled[v] and bfs.leaves[v]) {
                scheduled[v] = true;
                candidate.push_back(v);
            }
        }
    }

    bool found() const
    {
        return max_excentricity == min_excentricity * 2;
    }

    Diameter(std::size_t o) noexcept :
      done(o, false), scheduled(o, false), bfs(o), order(o), min_excentricity(o)
    {}

    std::deque<std::size_t> candidate;
    std::vector<bool>       done;
    std::vector<bool>       scheduled;
    BFS                     bfs;
    std::size_t             order            = 0;
    std::size_t             max_excentricity = 0;
    std::size_t             min_excentricity = 0;
    std::size_t             runs             = 0;
};

};
