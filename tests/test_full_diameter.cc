#include <gtest/gtest.h>

#include "full_diameter.h"
#include "graph.h"
#include "helpers.h"

using test_helpers::make_path;
using test_helpers::make_two_triangles;

TEST(FullDiameterTest, SingleVertex)
{
    Graph g(1);
    auto  result = full::run(g);

    EXPECT_EQ(result.diameter, 0U);
    EXPECT_EQ(result.bfs_runs, 1U);
    EXPECT_TRUE(result.connected);
}

TEST(FullDiameterTest, PathFive)
{
    Graph g      = make_path(5);
    auto  result = full::run(g);

    EXPECT_EQ(result.diameter, 4U);
    EXPECT_EQ(result.bfs_runs, 5U);
    EXPECT_TRUE(result.connected);
}

TEST(FullDiameterTest, Disconnected)
{
    Graph g      = make_two_triangles(/*with_bridge=*/false);
    auto  result = full::run(g);
    EXPECT_FALSE(result.connected);
}

// On a path of 5: diameter=4, radius=2. After 3 BFS runs (from 0, 1, 2)
// max_ecc==4 and min_ecc==2 → 4 == 2*2 → early termination fires.
TEST(FullDiameterTest, WithRadiusEarlyTerminationOnPath)
{
    Graph g      = make_path(5);
    auto  result = full::run<true>(g);

    EXPECT_EQ(result.diameter, 4U);
    EXPECT_EQ(result.radius, 2U);
    EXPECT_LT(result.bfs_runs, 5U);
    EXPECT_TRUE(result.connected);
}

TEST(FullDiameterTest, EmptyGraph)
{
    Graph g(0);
    auto  result = full::run(g);

    EXPECT_EQ(result.diameter, 0U);
    EXPECT_EQ(result.bfs_runs, 0U);
    EXPECT_TRUE(result.connected);
}
