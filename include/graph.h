/*
 * Graph data structure
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <span>
#include <vector>

class Graph
{
public:
    Graph() = default;
    explicit Graph(std::size_t graph_order) : vertices_(graph_order) {}

    [[nodiscard]] std::size_t order() const noexcept
    {
        return vertices_.size();
    }

    [[nodiscard]] std::span<const std::size_t> operator[](std::size_t v) const noexcept
    {
        return vertices_[v];
    }

    void reset(std::size_t graph_order)
    {
        vertices_.clear();
        vertices_.resize(graph_order);
    }

    // Silently drops out-of-range indices so callers (e.g. nde::load) can
    // forward malformed input without preflight checks. reserve_edges below
    // follows the same policy.
    void add_edge(std::size_t v0, std::size_t v1)
    {
        if (v0 < order() and v1 < order()) {
            vertices_[v0].push_back(v1);
            vertices_[v1].push_back(v0);
        }
    }

    void reserve_edges(std::size_t v, std::size_t degree)
    {
        if (v < order()) {
            vertices_[v].reserve(degree);
        }
    }

    void sort_successors()
    {
        for (auto& s : vertices_) {
            std::ranges::sort(s);
        }
    }

private:
    std::vector<std::vector<std::size_t>> vertices_;
};
