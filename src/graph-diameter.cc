/*
 * Computing graph diameter
 */

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

    smart::Diameter diameter { graph.order() };
    std::cout << "Order: " << graph.order() << "\n";
    std::cout << "Diameter: " << diameter(graph) << "\n";
    std::cout << "Radius: " << diameter.min_excentricity << "\n";
    std::cout << "Runs: " << diameter.runs << "\n";
}
