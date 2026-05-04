/*
 * Breath First Traversal
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <span>
#include <vector>

#include "graph.h"

class BFS
{
public:
    explicit BFS(std::size_t order) : dist_(order), closed_(order, 0), leaves_(order, 1) {}

    void operator()(const Graph& graph, std::size_t source)
    {
        dist_[source]   = 0;
        closed_[source] = 1;
        queue_.push_back(source);
        while (not queue_.empty()) {
            ++visited_vertices_;
            auto&& v = queue_.front();
            queue_.pop_front();
            last_ = v;
            for (auto&& s : graph[v]) {
                if (closed_[s] == 0) {
                    closed_[s]    = 1;
                    dist_[s]      = dist_[v] + 1;
                    max_distance_ = dist_[s];
                    leaves_[v]    = 0;
                    queue_.push_back(s);
                }
            }
        }
    }

    void reset() noexcept
    {
        queue_.clear();
        max_distance_     = 0;
        visited_vertices_ = 0;
        last_             = 0;
        std::ranges::fill(closed_, char { 0 });
        std::ranges::fill(leaves_, char { 1 });
    }

    [[nodiscard]] std::size_t order() const noexcept
    {
        return dist_.size();
    }
    [[nodiscard]] std::span<const std::size_t> distances() const noexcept
    {
        return dist_;
    }
    [[nodiscard]] std::size_t distance(std::size_t v) const noexcept
    {
        return dist_[v];
    }
    [[nodiscard]] std::size_t max_distance() const noexcept
    {
        return max_distance_;
    }
    [[nodiscard]] std::size_t visited() const noexcept
    {
        return visited_vertices_;
    }
    [[nodiscard]] std::size_t last_visited() const noexcept
    {
        return last_;
    }
    [[nodiscard]] bool was_closed(std::size_t v) const noexcept
    {
        return closed_[v] != 0;
    }
    [[nodiscard]] bool is_leaf(std::size_t v) const noexcept
    {
        return leaves_[v] != 0;
    }

private:
    std::deque<std::size_t>  queue_;
    std::vector<std::size_t> dist_;
    std::vector<char>        closed_;
    std::vector<char>        leaves_;
    std::size_t              max_distance_     = 0;
    std::size_t              visited_vertices_ = 0;
    std::size_t              last_             = 0;
};
