#include <gtest/gtest.h>

#include <algorithm>

#include "bfs.h"
#include "graph.h"
#include "helpers.h"

using test_helpers::make_complete;
using test_helpers::make_path;

TEST(BFSTest, SingleVertex)
{
    Graph g(1);
    BFS   bfs(1);
    bfs(g, 0);

    EXPECT_EQ(bfs.max_distance(), 0U);
    EXPECT_EQ(bfs.visited(), 1U);
    EXPECT_EQ(bfs.distance(0), 0U);
    EXPECT_EQ(bfs.last_visited(), 0U);
}

TEST(BFSTest, PathThree)
{
    Graph g = make_path(3);
    BFS   bfs(3);
    bfs(g, 0);

    EXPECT_TRUE(std::ranges::equal(bfs.distances(), std::initializer_list<std::size_t> { 0, 1, 2 }));
    EXPECT_EQ(bfs.max_distance(), 2U);
    EXPECT_EQ(bfs.visited(), 3U);
    EXPECT_EQ(bfs.last_visited(), 2U);
}

TEST(BFSTest, CompleteFour)
{
    Graph g = make_complete(4);
    BFS   bfs(4);
    bfs(g, 0);

    EXPECT_EQ(bfs.max_distance(), 1U);
    EXPECT_EQ(bfs.visited(), 4U);
    EXPECT_EQ(bfs.distance(0), 0U);
    for (std::size_t v = 1; v < 4; ++v) {
        EXPECT_EQ(bfs.distance(v), 1U);
    }
}

TEST(BFSTest, Disconnected)
{
    Graph g(4);
    g.add_edge(0, 1);
    g.add_edge(2, 3);
    g.sort_successors();

    BFS bfs(4);
    bfs(g, 0);

    EXPECT_EQ(bfs.visited(), 2U);
    EXPECT_EQ(bfs.max_distance(), 1U);
    EXPECT_TRUE(bfs.was_closed(0));
    EXPECT_TRUE(bfs.was_closed(1));
    EXPECT_FALSE(bfs.was_closed(2));
    EXPECT_FALSE(bfs.was_closed(3));
}

TEST(BFSTest, ResetClearsLastVisited)
{
    Graph g = make_path(3);
    BFS   bfs(3);
    bfs(g, 0);
    ASSERT_EQ(bfs.last_visited(), 2U);

    bfs.reset();
    EXPECT_EQ(bfs.last_visited(), 0U);
}
