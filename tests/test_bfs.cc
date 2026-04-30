#include <gtest/gtest.h>

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

    EXPECT_EQ(bfs.max_distance, 0u);
    EXPECT_EQ(bfs.visited_vertices, 1u);
    EXPECT_EQ(bfs.dist[0], 0u);
    EXPECT_EQ(bfs.last, 0u);
}

TEST(BFSTest, PathThree)
{
    Graph g = make_path(3);
    BFS   bfs(3);
    bfs(g, 0);

    EXPECT_EQ(bfs.dist, (std::vector<std::size_t> { 0, 1, 2 }));
    EXPECT_EQ(bfs.max_distance, 2u);
    EXPECT_EQ(bfs.visited_vertices, 3u);
    EXPECT_EQ(bfs.last, 2u);
}

TEST(BFSTest, CompleteFour)
{
    Graph g = make_complete(4);
    BFS   bfs(4);
    bfs(g, 0);

    EXPECT_EQ(bfs.max_distance, 1u);
    EXPECT_EQ(bfs.visited_vertices, 4u);
    EXPECT_EQ(bfs.dist[0], 0u);
    for (std::size_t v = 1; v < 4; ++v) {
        EXPECT_EQ(bfs.dist[v], 1u);
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

    EXPECT_EQ(bfs.visited_vertices, 2u);
    EXPECT_EQ(bfs.max_distance, 1u);
    EXPECT_TRUE(bfs.closed[0]);
    EXPECT_TRUE(bfs.closed[1]);
    EXPECT_FALSE(bfs.closed[2]);
    EXPECT_FALSE(bfs.closed[3]);
}
