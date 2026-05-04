#include <gtest/gtest.h>

#include "full_diameter.h"
#include "graph.h"
#include "helpers.h"
#include "smart_diameter.h"

using test_helpers::make_complete;
using test_helpers::make_path;
using test_helpers::make_star;

namespace {

std::size_t full_diameter(const Graph& g)
{
    return full::run(g).diameter;
}

};  // namespace

TEST(SmartDiameterTest, AgreesPath)
{
    Graph           g = make_path(6);
    smart::Diameter d(6);
    auto            result = d(g);

    EXPECT_EQ(result.diameter, full_diameter(g));
    EXPECT_LE(result.bfs_runs, 6U);
}

TEST(SmartDiameterTest, AgreesK4)
{
    Graph           g = make_complete(4);
    smart::Diameter d(4);
    auto            result = d(g);

    EXPECT_EQ(result.diameter, full_diameter(g));
    EXPECT_EQ(result.diameter, 1U);
}

TEST(SmartDiameterTest, AgreesStar)
{
    Graph           g = make_star(7);
    smart::Diameter d(7);

    EXPECT_EQ(d(g).diameter, full_diameter(g));
}

// Verifies the explicit-source overload is honoured: the diameter result
// must be invariant of starting vertex on a connected graph.
TEST(SmartDiameterTest, ExplicitSourceUsesGivenVertex)
{
    Graph           g = make_path(6);
    smart::Diameter d(6, 2);  // explicit non-endpoint source
    auto            result = d(g);

    EXPECT_EQ(result.diameter, 5U);
    EXPECT_EQ(result.diameter, full_diameter(g));
}

TEST(SmartDiameterTest, EmptyGraph)
{
    Graph           g(0);
    smart::Diameter d(0);
    auto            result = d(g);

    EXPECT_EQ(result.diameter, 0U);
    EXPECT_EQ(result.bfs_runs, 0U);
}
