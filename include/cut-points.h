/*
 * Find cut points: can be a good starting point
 */

#pragma once

#include <tuple>
#include <unordered_set>
#include <vector>

#include "graph.h"

struct Cut_points
{
    std::unordered_set<std::size_t> results;

    void operator()(const Graph& graph)
    {
        using iter  = std::vector<std::size_t>::const_iterator;
        using state = std::tuple<std::size_t, long, iter>;

        auto order = graph.order();

        std::vector<long>        prefix(order);
        std::vector<long>        highest(order);
        std::vector<std::size_t> parents(order);
        std::vector<iter>        parents_iter(order);

        for (std::size_t i = 0; i < order; ++i) {
            prefix[i]  = -1;
            highest[i] = order;
            parents[i] = i;
        }

        long        counter       = 0;
        std::size_t root_children = 0;
        prefix[0]                 = counter;
        highest[0]                = counter;

        std::vector<state> stack;
        stack.emplace_back(0, -1, begin(graph[0]));

        while (not stack.empty()) {
            auto [v, ret, next] = stack.back();
            stack.pop_back();
            if (ret != -1) {
                if (parents[v] == v) {
                    ++root_children;
                } else {
                    if (ret >= prefix[v]) {
                        results.insert(v);
                    }
                    highest[v] = std::min(highest[v], ret);
                }
            }
            for (; next != end(graph[v]) and prefix[*next] != -1; ++next) {
                if (*next != parents[v]) {
                    highest[v] = std::min(highest[v], prefix[*next]);
                }
            }
            if (next != end(graph[v])) {
                if (next + 1 != end(graph[v])) {
                    stack.emplace_back(v, -1, next + 1);
                }
                ++counter;
                prefix[*next]       = counter;
                highest[*next]      = counter;
                parents[*next]      = v;
                parents_iter[*next] = next;
                stack.emplace_back(*next, -1, begin(graph[*next]));
            } else {
                if (parents[v] == v) {
                    ++root_children;
                    results.insert(v);
                    return;
                }
                stack.emplace_back(parents[v], highest[v], parents_iter[v]);
            }
        }
    }
};
