/*
 * Computing graph diameter
 */

#include <chrono>
#include <err.h>
#include <filesystem>
#include <iostream>
#include <unordered_set>

#include <cxxopts.hpp>

#include "bfs.h"
#include "cut-points.h"
#include "full_diameter.h"
#include "graph.h"
#include "nde.h"
#include "smart_diameter.h"

auto choose_start(const Graph& graph, auto&& cut_points)
{
    if (cut_points.empty()) {
        std::size_t v = 0;
        for (std::size_t i = 1; i < graph.order(); ++i) {
            if (graph[i].size() > graph[v].size()) {
                v = i;
            }
        }
        return v;
    }
    auto v = *begin(cut_points);
    for (auto&& i : cut_points) {
        if (graph[i].size() < graph[v].size()) {
            v = i;
        }
    }
    return v;
}

int main(int argc, char* argv[])
try {

    cxxopts::Options argParser(argv[0], "compute graph diameter");
    argParser.positional_help("[filename]");
    argParser.add_options()
        ("filename", "path to graph file in NDE format", cxxopts::value<std::string>())
        ("h,help", "print usage");
    argParser.parse_positional({ "filename" });

    auto options = argParser.parse(argc, argv);

    if (options.count("help") or options.count("filename") == 0) {
        errx(1, "%s", argParser.help().c_str());
    }

    fs::path filename = options["filename"].as<std::string>();

    auto graph = nde::load(filename);
    std::cout << "Order: " << graph.order() << "\n";

    // Compute cut-points
    Cut_points cut_points {};
    cut_points(graph);
    std::cout << "Number of cut-points: " << cut_points.results.size() << "\n";

    auto start = choose_start(graph, cut_points.results);
    std::cout << "Starting node: " << start << "\n";

    auto diameter = smart::Diameter(graph.order(), start);

    auto t0   = std::chrono::steady_clock::now();
    auto diam = diameter(graph);
    auto t1   = std::chrono::steady_clock::now();

    std::cout << "Diameter: " << diam << "\n";
    std::cout << "Runs: " << diameter.runs << " (+1 DFS for cut points)\n";
    std::cout << "Last change: " << diameter.last_change << "\n";
    std::cout << "Diametral vertex: " << diameter.diam_vertex << "\n";
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>((t1 - t0)).count() << "ms\n";
} catch (std::exception& e) {
    errx(1, "%s", e.what());
}
