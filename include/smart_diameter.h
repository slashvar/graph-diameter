/*
 * smart diamter: compute the diameter trying to skip useless nodes
 */

#pragma once

#include <algorithm>
#include <deque>
#include <vector>

#include "bfs.h"
#include "graph.h"

namespace smart {

struct Diameter
{

    std::size_t operator()(const Graph& graph)
    {
        candidate.push_back(choose_source(graph));
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
            if (not done[bfs.last] and bfs.dist[bfs.last] > threshold()) {
                done[bfs.last] = true;
                bfs.reset();
                bfs(graph, bfs.last);
                max_excentricity = std::max(max_excentricity, bfs.max_distance);
                min_excentricity = std::min(min_excentricity, bfs.max_distance);
                ++runs;
            }
            populate();
        }
        return max_excentricity;
    }

    std::size_t threshold() const
    {
        if (min_excentricity < max_excentricity) {
            return min_excentricity + (max_excentricity - min_excentricity) / 2;
        }
        return max_excentricity / 2;
    }

    void populate()
    {
        for (std::size_t v = 0; v < order; ++v) {
            dist[v] = std::max(bfs.dist[v], dist[v]);
            if (not done[v] and not scheduled[v] and bfs.leaves[v] and bfs.dist[v] > threshold()
                and bfs.dist[v] >= dist[v]) {
                scheduled[v] = true;
                candidate.push_back(v);
            }
        }
        auto eliminate = [&done = done, &leaves = this->bfs.leaves](auto v) {
            done[v] = done[v] or not leaves[v];
            return done[v];
        };
        candidate.erase(std::remove_if(begin(candidate), end(candidate), eliminate), end(candidate));
    }

    std::size_t choose_source(const Graph& g) const
    {
        std::size_t d   = g[0].size();
        std::size_t src = 0;
        for (std::size_t v = 1; v < g.order(); ++v) {
            if (g[v].size() < d) {
                d   = g[v].size();
                src = v;
            }
        }
        return src;
    }

    bool found() const
    {
        return max_excentricity >= min_excentricity * 2;
    }

    Diameter(std::size_t o) noexcept :
      done(o, false), scheduled(o, false), dist(o), bfs(o), order(o), min_excentricity(o)
    {}

    std::deque<std::size_t>  candidate;
    std::vector<bool>        done;
    std::vector<bool>        scheduled;
    std::vector<std::size_t> dist;
    BFS                      bfs;
    std::size_t              order            = 0;
    std::size_t              max_excentricity = 0;
    std::size_t              min_excentricity = 0;
    std::size_t              runs             = 0;
};

};
