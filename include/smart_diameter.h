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
    void run(const Graph& graph, std::size_t v)
    {
        ++runs;
        done[v] = true;
        bfs.reset();
        bfs(graph, v);
        if (bfs.max_distance > max_excentricity) {
            max_excentricity = bfs.max_distance;
            last_change      = runs;
            diam_vertex      = v;
        }
        min_excentricity = std::min(min_excentricity, bfs.max_distance);
    }

    std::size_t operator()(const Graph& graph)
    {
        if (candidate.empty()) {
            candidate.push_back(choose_source(graph));
        }

        while (not candidate.empty() and not found()) {
            auto v = candidate.front();
            candidate.pop_front();
            run(graph, v);

            if (not done[bfs.last] and bfs.dist[bfs.last] > threshold() and bfs.dist[bfs.last] >= dist[bfs.last]) {
                run(graph, bfs.last);
                if (not done[bfs.last] and bfs.dist[bfs.last] > threshold()
                    and bfs.dist[bfs.last] >= dist[bfs.last]) {
                    run(graph, bfs.last);
                }
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
        if (source == order) {
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
        return source;
    }

    bool found() const
    {
        return max_excentricity >= min_excentricity * 2;
    }

    template <typename Range>
    void add_candidates(Range&& set)
    {
        candidate.insert(end(candidate), begin(set), end(set));
    }

    Diameter(std::size_t o, std::size_t src) noexcept :
      done(o, false), scheduled(o, false), dist(o), bfs(o), order(o), min_excentricity(o), source(src)
    {}

    Diameter(std::size_t o) noexcept : Diameter(o, o) {}

    std::deque<std::size_t>  candidate;
    std::vector<bool>        done;
    std::vector<bool>        scheduled;
    std::vector<std::size_t> dist;
    BFS                      bfs;
    std::size_t              order            = 0;
    std::size_t              max_excentricity = 0;
    std::size_t              min_excentricity = 0;
    std::size_t              runs             = 0;
    std::size_t              last_change      = 0;
    std::size_t              diam_vertex      = 0;
    std::size_t              source;
};

};
