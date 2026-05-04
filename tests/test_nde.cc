#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <sstream>
#include <stdexcept>

#include "graph.h"
#include "helpers.h"
#include "nde.h"

namespace fs = std::filesystem;
using test_helpers::make_path;
using test_helpers::TempNDEFile;

TEST(NdeTest, LoadValidPath)
{
    TempNDEFile tmp("3\n0 1\n1 2\n2 1\n0 1\n1 2\n");
    Graph       g = nde::load(tmp.path);

    Graph expected = make_path(3);
    ASSERT_EQ(g.order(), expected.order());
    for (std::size_t v = 0; v < g.order(); ++v) {
        EXPECT_TRUE(std::ranges::equal(g[v], expected[v]));
    }
}

TEST(NdeTest, LoadMissingFile)
{
    EXPECT_THROW((void)nde::load("/no/such/file.nde"), fs::filesystem_error);
}

TEST(NdeTest, LoadMalformed)
{
    TempNDEFile tmp("3\n0 1\n");
    EXPECT_THROW((void)nde::load(tmp.path), std::invalid_argument);
}

TEST(NdeTest, SerializeRoundtrip)
{
    Graph              original = make_path(4);
    std::ostringstream out;
    nde::serialize(original, out);

    TempNDEFile tmp(out.str());
    Graph       loaded = nde::load(tmp.path);

    ASSERT_EQ(loaded.order(), original.order());
    for (std::size_t v = 0; v < original.order(); ++v) {
        EXPECT_TRUE(std::ranges::equal(loaded[v], original[v]));
    }
}

TEST(NdeTest, LoadFromStringStream)
{
    std::istringstream input("3\n0 1\n1 2\n2 1\n0 1\n1 2\n");
    Graph              g        = nde::load(input);
    Graph              expected = make_path(3);

    ASSERT_EQ(g.order(), expected.order());
    for (std::size_t v = 0; v < g.order(); ++v) {
        EXPECT_TRUE(std::ranges::equal(g[v], expected[v]));
    }
}

TEST(NdeTest, LoadRejectsOutOfRangeVertexInDegreeSection)
{
    // Order is 3 but the degree section claims vertex id 5.
    std::istringstream input("3\n0 1\n5 2\n2 1\n0 1\n1 2\n");
    EXPECT_THROW((void)nde::load(input), std::invalid_argument);
}
