/*
 * Graph data structure
 */

#pragma once

#include <vector>

class Graph
{
public:
    std::size_t order() const
    {
        return vertices_.size();
    }

    const std::vector<std::size_t>& operator[](std::size_t v) const
    {
        return vertices_[v];
    }

    void reset(std::size_t graph_order)
    {
        vertices_.clear();
        vertices_.resize(graph_order);
    }

    void add_edge(std::size_t v0, std::size_t v1)
    {
        if (v0 < order() and v1 < order()) {
            vertices_[v0].push_back(v1);
            vertices_[v1].push_back(v0);
        }
    }

    void reserve_edges(std::size_t v, std::size_t degree)
    {
        vertices_[v].reserve(degree);
    }

public:
    Graph() = default;
    Graph(std::size_t graph_order) : vertices_(graph_order) {}

private:
    std::vector<std::vector<std::size_t>> vertices_;
};
