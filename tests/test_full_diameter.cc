#include <gtest/gtest.h>

#include "full_diameter.h"
#include "graph.h"
#include "helpers.h"

using test_helpers::make_path;
using test_helpers::make_two_triangles;

TEST(FullDiameterTest, SingleVertex)
{
    Graph                 g(1);
    full::Diameter<false> d;
    auto                  result = d(g);

    EXPECT_EQ(result, 0u);
    EXPECT_EQ(d.runs, 1u);
    EXPECT_TRUE(d.connected);
}

TEST(FullDiameterTest, PathFive)
{
    Graph                 g = make_path(5);
    full::Diameter<false> d;
    auto                  result = d(g);

    EXPECT_EQ(result, 4u);
    EXPECT_EQ(d.runs, 5u);
    EXPECT_TRUE(d.connected);
}

TEST(FullDiameterTest, Disconnected)
{
    Graph                 g = make_two_triangles(/*with_bridge=*/false);
    full::Diameter<false> d;
    d(g);

    EXPECT_FALSE(d.connected);
}
