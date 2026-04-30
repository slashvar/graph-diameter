#include <gtest/gtest.h>

#include "graph.h"
#include "helpers.h"

TEST(GraphTest, OrderReset)
{
    Graph g(5);
    EXPECT_EQ(g.order(), 5u);
    g.reset(3);
    EXPECT_EQ(g.order(), 3u);
    for (std::size_t v = 0; v < g.order(); ++v) {
        EXPECT_TRUE(g[v].empty());
    }
}

TEST(GraphTest, AddEdgeInBounds)
{
    Graph g(3);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.sort_successors();

    EXPECT_EQ(g[0], (std::vector<std::size_t> { 1 }));
    EXPECT_EQ(g[1], (std::vector<std::size_t> { 0, 2 }));
    EXPECT_EQ(g[2], (std::vector<std::size_t> { 1 }));
}

TEST(GraphTest, AddEdgeOutOfBounds)
{
    Graph g(3);
    g.add_edge(0, 99);
    g.add_edge(99, 0);
    g.add_edge(50, 60);
    for (std::size_t v = 0; v < g.order(); ++v) {
        EXPECT_TRUE(g[v].empty());
    }
}

TEST(GraphTest, SortSuccessors)
{
    Graph g(4);
    g.add_edge(0, 3);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.sort_successors();
    EXPECT_EQ(g[0], (std::vector<std::size_t> { 1, 2, 3 }));
}

TEST(GraphTest, ReserveEdgesOutOfBoundsIsSafe)
{
    Graph g(3);
    g.reserve_edges(99, 5);
    for (std::size_t v = 0; v < g.order(); ++v) {
        EXPECT_TRUE(g[v].empty());
    }
}
