/*
 * Computing graph diameter
 */

#include <iostream>
#include <filesystem>
#include <err.h>

#include "graph.h"
#include "nde.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        errx(1, "missing file name");
    }
    auto graph = nde::load(argv[1]);

    auto order = graph.order();

    std::cout << order<< "\n";
    for (std::size_t v = 0; v < order; ++v) {
        std::cout << v << " " << graph[v].size() << "\n";
    }
    for (std::size_t v = 0; v < order; ++v) {
        for (auto&& s : graph[v]) {
            if (s > v) {
                std::cout << v << " " << s << "\n";
            }
        }
    }
}
