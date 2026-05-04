#include <gtest/gtest.h>

#include <type_traits>

#include "graph.h"
#include "helpers.h"

using test_helpers::adj_equal;

TEST(GraphTest, OrderReset)
{
    Graph g(5);
    EXPECT_EQ(g.order(), 5U);
    g.reset(3);
    EXPECT_EQ(g.order(), 3U);
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

    EXPECT_TRUE(adj_equal(g[0], { 1 }));
    EXPECT_TRUE(adj_equal(g[1], { 0, 2 }));
    EXPECT_TRUE(adj_equal(g[2], { 1 }));
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
    EXPECT_TRUE(adj_equal(g[0], { 1, 2, 3 }));
}

TEST(GraphTest, ReserveEdgesOutOfBoundsIsSafe)
{
    Graph g(3);
    g.reserve_edges(99, 5);
    for (std::size_t v = 0; v < g.order(); ++v) {
        EXPECT_TRUE(g[v].empty());
    }
}

static_assert(!std::is_convertible_v<std::size_t, Graph>,
              "Graph(std::size_t) must be explicit to prevent implicit size-to-Graph conversions");
