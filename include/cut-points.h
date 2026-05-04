/*
 * Find cut points (articulation points) via iterative DFS.
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <optional>
#include <span>
#include <unordered_set>
#include <vector>

#include "graph.h"

class CutPoints
{
public:
    // Run the iterative DFS articulation-point algorithm starting at vertex 0.
    // Only finds articulation points in the connected component reachable from
    // vertex 0; other components are silently ignored.
    void operator()(const Graph& graph)
    {
        using neighbors = std::span<const std::size_t>;
        using iter      = neighbors::iterator;

        struct Parent
        {
            std::size_t vertex;
            iter        from_iter;  // iterator at which the parent descended into this node
        };

        struct Frame
        {
            std::size_t                vertex;
            std::optional<std::size_t> ret;
            iter                       next;
        };

        const auto order = graph.order();
        if (order == 0) {
            return;
        }

        std::vector<char>                  discovered(order, 0);
        std::vector<std::size_t>           prefix(order);
        std::vector<std::size_t>           highest(order);
        std::vector<std::optional<Parent>> parents(order);

        std::size_t           counter       = 0;
        std::size_t           root_children = 0;
        constexpr std::size_t root          = 0;

        discovered[root] = 1;
        prefix[root]     = counter;
        highest[root]    = counter;

        // Process the return value of a finished child (low-link propagation
        // to the parent, or root_children bookkeeping at the root).
        auto receive_return = [&](const Frame& frame) {
            if (not frame.ret) {
                return;
            }
            if (not parents[frame.vertex]) {
                ++root_children;
                return;
            }
            if (*frame.ret >= prefix[frame.vertex]) {
                results_.insert(frame.vertex);
            }
            highest[frame.vertex] = std::min(highest[frame.vertex], *frame.ret);
        };

        // Advance frame.next past already-discovered (non-tree) neighbors,
        // updating low-link from their discovery times.
        auto skip_seen = [&](Frame& frame, iter end_it) {
            while (frame.next != end_it and discovered[*frame.next] != 0) {
                const bool is_parent = parents[frame.vertex] and parents[frame.vertex]->vertex == *frame.next;
                if (not is_parent) {
                    highest[frame.vertex] = std::min(highest[frame.vertex], prefix[*frame.next]);
                }
                ++frame.next;
            }
        };

        std::vector<Frame> stack;
        stack.push_back({ root, std::nullopt, graph[root].begin() });

        while (not stack.empty()) {
            auto frame = stack.back();
            stack.pop_back();
            receive_return(frame);

            const auto end_it = graph[frame.vertex].end();
            skip_seen(frame, end_it);

            if (frame.next != end_it) {
                const auto child     = *frame.next;
                const auto next_iter = frame.next + 1;
                if (next_iter != end_it) {
                    stack.push_back({ frame.vertex, std::nullopt, next_iter });
                }
                ++counter;
                discovered[child] = 1;
                prefix[child]     = counter;
                highest[child]    = counter;
                parents[child]    = Parent { .vertex = frame.vertex, .from_iter = frame.next };
                stack.push_back({ child, std::nullopt, graph[child].begin() });
            } else if (const auto& parent = parents[frame.vertex]; parent) {
                stack.push_back({ parent->vertex, highest[frame.vertex], parent->from_iter });
            } else {
                if (root_children >= 2) {
                    results_.insert(frame.vertex);
                }
                return;
            }
        }
    }

    [[nodiscard]] const std::unordered_set<std::size_t>& articulation_points() const noexcept
    {
        return results_;
    }

private:
    std::unordered_set<std::size_t> results_;
};
