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
    Graph      g = make_cycle(5);
    Cut_points cp;
    cp(g);

    EXPECT_TRUE(cp.results.empty()) << "cycle should have no articulation points";
}

TEST(CutPointsTest, PathInteriorAreCutPoints)
{
    Graph      g = make_path(5);
    Cut_points cp;
    cp(g);

    std::unordered_set<std::size_t> expected { 1, 2, 3 };
    EXPECT_EQ(cp.results, expected);
}

TEST(CutPointsTest, BridgeEndpointsAreCutPoints)
{
    Graph      g = make_two_triangles(/*with_bridge=*/true);
    Cut_points cp;
    cp(g);

    std::unordered_set<std::size_t> expected { 2, 3 };
    EXPECT_EQ(cp.results, expected);
}
