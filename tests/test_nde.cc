#include <gtest/gtest.h>

#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "graph.h"
#include "helpers.h"
#include "nde.h"

namespace fs = std::filesystem;
using test_helpers::graphs_equal;
using test_helpers::make_path;
using test_helpers::TempNDEFile;

namespace {

constexpr std::string_view PATH3_NDE = "3\n0 1\n1 2\n2 1\n0 1\n1 2\n";

};  // namespace

TEST(NdeTest, LoadValidPath)
{
    TempNDEFile tmp(PATH3_NDE);
    Graph       g = nde::load(tmp.path);

    EXPECT_TRUE(graphs_equal(g, make_path(3)));
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

    std::istringstream in(out.str());
    Graph              loaded = nde::load(in);

    EXPECT_TRUE(graphs_equal(loaded, original));
}

TEST(NdeTest, LoadFromStringStream)
{
    std::istringstream input { std::string(PATH3_NDE) };
    Graph              g = nde::load(input);

    EXPECT_TRUE(graphs_equal(g, make_path(3)));
}

TEST(NdeTest, LoadRejectsOutOfRangeVertexInDegreeSection)
{
    // Order is 3 but the degree section claims vertex id 5.
    std::istringstream input("3\n0 1\n5 2\n2 1\n0 1\n1 2\n");
    EXPECT_THROW((void)nde::load(input), std::invalid_argument);
}
