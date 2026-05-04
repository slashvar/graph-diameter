#include <gtest/gtest.h>

#include <unordered_set>

#include "cut-points.h"
#include "graph.h"
#include "helpers.h"

using test_helpers::make_cycle;
using test_helpers::make_path;
using test_helpers::make_two_triangles;

TEST(CutPointsTest, CycleNoCutPoints)
{
    Graph     g = make_cycle(5);
    CutPoints cp;
    cp(g);

    EXPECT_TRUE(cp.articulation_points().empty()) << "cycle should have no articulation points";
}

TEST(CutPointsTest, PathInteriorAreCutPoints)
{
    Graph     g = make_path(5);
    CutPoints cp;
    cp(g);

    std::unordered_set<std::size_t> expected { 1, 2, 3 };
    EXPECT_EQ(cp.articulation_points(), expected);
}

TEST(CutPointsTest, BridgeEndpointsAreCutPoints)
{
    Graph     g = make_two_triangles(/*with_bridge=*/true);
    CutPoints cp;
    cp(g);

    std::unordered_set<std::size_t> expected { 2, 3 };
    EXPECT_EQ(cp.articulation_points(), expected);
}

// New guard: previously order==0 was UB (out-of-bounds prefix[0] = ...).
TEST(CutPointsTest, EmptyGraph)
{
    Graph     g(0);
    CutPoints cp;
    cp(g);
    EXPECT_TRUE(cp.articulation_points().empty());
}

// Characterization: when vertex 0 has no neighbors, the algorithm only
// explores its (singleton) component and reports no articulation points.
TEST(CutPointsTest, IsolatedRoot)
{
    Graph     g(3);  // 3 vertices, no edges
    CutPoints cp;
    cp(g);
    EXPECT_TRUE(cp.articulation_points().empty());
}
