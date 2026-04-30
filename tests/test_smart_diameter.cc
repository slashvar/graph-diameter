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
    full::Diameter<false> d;
    return d(g);
}

}; // namespace

TEST(SmartDiameterTest, AgreesPath)
{
    Graph           g = make_path(6);
    smart::Diameter d(6);
    auto            smart_result = d(g);
    auto            full_result  = full_diameter(g);

    EXPECT_EQ(smart_result, full_result);
    EXPECT_LE(d.runs, 6u);
}

TEST(SmartDiameterTest, AgreesK4)
{
    Graph           g = make_complete(4);
    smart::Diameter d(4);
    auto            result = d(g);
    EXPECT_EQ(result, full_diameter(g));
    EXPECT_EQ(result, 1u);
}

TEST(SmartDiameterTest, AgreesStar)
{
    Graph           g = make_star(7);
    smart::Diameter d(7);
    EXPECT_EQ(d(g), full_diameter(g));
}
