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
            auto&& v = queue.front();
            queue.pop_front();
            for (auto&& s : graph[v]) {
                if (not closed[s]) {
                    closed[s] = true;
                    dist[s]   = dist[v] + 1;
                    queue.push_back(s);
                    max_distance = dist[s];
                }
            }
        }
    }

    BFS(std::size_t order) noexcept : dist(order), closed(order, false) {}

    std::deque<std::size_t>  queue;
    std::vector<std::size_t> dist;
    std::vector<bool>        closed;
    std::size_t              max_distance = 0;
};
