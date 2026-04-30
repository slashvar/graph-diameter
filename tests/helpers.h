#pragma once

#include <unistd.h>

#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>

#include "graph.h"

namespace fs = std::filesystem;

namespace test_helpers {

inline Graph make_path(std::size_t n)
{
    Graph g(n);
    for (std::size_t i = 0; i + 1 < n; ++i) {
        g.add_edge(i, i + 1);
    }
    g.sort_successors();
    return g;
}

inline Graph make_cycle(std::size_t n)
{
    Graph g = make_path(n);
    if (n >= 3) {
        g.add_edge(0, n - 1);
        g.sort_successors();
    }
    return g;
}

inline Graph make_complete(std::size_t n)
{
    Graph g(n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i + 1; j < n; ++j) {
            g.add_edge(i, j);
        }
    }
    g.sort_successors();
    return g;
}

inline Graph make_star(std::size_t n)
{
    Graph g(n);
    for (std::size_t i = 1; i < n; ++i) {
        g.add_edge(0, i);
    }
    g.sort_successors();
    return g;
}

inline Graph make_two_triangles(bool with_bridge)
{
    Graph g(6);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 0);
    g.add_edge(3, 4);
    g.add_edge(4, 5);
    g.add_edge(5, 3);
    if (with_bridge) {
        g.add_edge(2, 3);
    }
    g.sort_successors();
    return g;
}

struct TempNDEFile
{
    fs::path path;

    explicit TempNDEFile(std::string_view body)
    {
        std::string templ = (fs::temp_directory_path() / "graph-diameter-test-XXXXXX").string();
        int         fd    = ::mkstemp(templ.data());
        ::write(fd, body.data(), body.size());
        ::close(fd);
        path = templ;
    }

    ~TempNDEFile()
    {
        std::error_code ec;
        fs::remove(path, ec);
    }

    TempNDEFile(const TempNDEFile&)            = delete;
    TempNDEFile& operator=(const TempNDEFile&) = delete;
};

};  // namespace test_helpers
