/*
 * Computing graph diameter
 */

#include <chrono>
#include <err.h>
#include <filesystem>
#include <iostream>
#include <unordered_set>

#include "bfs.h"
#include "cut-points.h"
#include "full_diameter.h"
#include "graph.h"
#include "nde.h"
#include "smart_diameter.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        errx(1, "missing file name");
    }

    auto graph = nde::load(argv[1]);
    std::cout << "Order: " << graph.order() << "\n";

    // Compute cut-points
    Cut_points cut_points {};
    cut_points(graph);

    std::cout << "Cut points:";
    for (auto&& v : cut_points.results) {
        std::cout << " " << v;
    }
    std::cout << "\n";

    auto diameter = argc > 2 ? smart::Diameter(graph.order(), std::atoi(argv[2])) : smart::Diameter(graph.order());

    auto t0   = std::chrono::steady_clock::now();
    auto diam = diameter(graph);
    auto t1   = std::chrono::steady_clock::now();

    std::cout << "Diameter: " << diam << "\n";
    std::cout << "Runs: " << diameter.runs << "\n";
    std::cout << "Last change: " << diameter.last_change << "\n";
    std::cout << "Diametral vertex: " << diameter.diam_vertex << "\n";
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>((t1 - t0)).count() << "ms\n";
}
