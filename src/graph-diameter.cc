/*
 * Computing graph diameter
 */

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <cxxopts.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <span>
#include <unordered_set>

#include "cut-points.h"
#include "graph.h"
#include "nde.h"
#include "smart_diameter.h"

namespace {

[[nodiscard]] std::size_t choose_start(const Graph& graph, const std::unordered_set<std::size_t>& cut_points)
{
    if (cut_points.empty()) {
        std::size_t best = 0;
        for (std::size_t v = 1; v < graph.order(); ++v) {
            if (graph[v].size() > graph[best].size()) {
                best = v;
            }
        }
        return best;
    }
    auto best = *cut_points.begin();
    for (auto v : cut_points) {
        if (graph[v].size() < graph[best].size()) {
            best = v;
        }
    }
    return best;
}

};  // namespace

int main(int argc, char* argv[])
try {
    const std::span  args(argv, static_cast<std::size_t>(argc));
    cxxopts::Options argParser(args[0], "compute graph diameter");
    argParser.positional_help("[filename]");

    // clang-format off
    argParser.add_options()
        ("filename", "path to graph file in NDE format", cxxopts::value<std::string>())
        ("h,help", "print usage");
    // clang-format on
    argParser.parse_positional({ "filename" });

    const auto options = argParser.parse(argc, argv);

    if (options.count("help") != 0 or options.count("filename") == 0) {
        std::cerr << argParser.help();
        return EXIT_FAILURE;
    }

    const fs::path filename = options["filename"].as<std::string>();

    const auto graph = nde::load(filename);
    std::cout << "Order: " << graph.order() << "\n";

    CutPoints cut_points;
    cut_points(graph);
    std::cout << "Number of cut-points: " << cut_points.articulation_points().size() << "\n";

    const auto start = choose_start(graph, cut_points.articulation_points());
    std::cout << "Starting node: " << start << "\n";

    smart::Diameter solver(graph.order(), start);

    const auto t0     = std::chrono::steady_clock::now();
    const auto result = solver(graph);
    const auto t1     = std::chrono::steady_clock::now();

    std::cout << "Diameter: " << result.diameter << "\n";
    std::cout << "Runs: " << result.bfs_runs << " (+1 DFS for cut points)\n";
    std::cout << "Last change: " << result.last_change << "\n";
    std::cout << "Diametral vertex: " << result.diameter_vertex << "\n";
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms\n";

    return EXIT_SUCCESS;
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
}
