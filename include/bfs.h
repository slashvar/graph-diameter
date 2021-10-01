/*
 * Breath First Traversal
 */

#pragma once

#include <deque>
#include <vector>

#include "graph.h"

struct BFS
{
    void operator()(const Graph& graph, std::size_t source)
    {
        dist[source] = 0;
        queue.push_back(source);
        closed[source] = true;
        while (not queue.empty()) {
            ++visited_vertices;
            auto&& v = queue.front();
            queue.pop_front();
            last = v;
            for (auto&& s : graph[v]) {
                if (not closed[s]) {
                    closed[s] = true;
                    dist[s]   = dist[v] + 1;
                    queue.push_back(s);
                    max_distance = dist[s];
                    leaves[v]    = false;
                }
            }
        }
    }

    BFS(std::size_t order) noexcept : dist(order), closed(order, false), leaves(order, true) {}

    void reset()
    {
        queue.clear();
        max_distance     = 0;
        visited_vertices = 0;
        for (auto&& c : closed) {
            c = false;
        }
        for (auto&& l : leaves) {
            l = true;
        }
    }

    std::deque<std::size_t>  queue;
    std::vector<std::size_t> dist;
    std::vector<bool>        closed;
    std::vector<bool>        leaves;
    std::size_t              max_distance     = 0;
    std::size_t              visited_vertices = 0;
    std::size_t              last;
};
