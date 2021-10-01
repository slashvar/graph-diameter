/*
 * Computing graph diameter
 */

#include <chrono>
#include <err.h>
#include <filesystem>
#include <iostream>

#include "bfs.h"
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

    smart::Diameter diameter { graph.order() };

    auto t0   = std::chrono::steady_clock::now();
    auto diam = diameter(graph);
    auto t1   = std::chrono::steady_clock::now();

    std::cout << "Diameter: " << diam << "\n";
    std::cout << "Runs: " << diameter.runs << "\n";
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>((t1 - t0)).count() << "ms\n";
}
