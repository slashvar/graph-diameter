/*
 * Computing graph diameter
 */

#include <err.h>
#include <filesystem>
#include <iostream>

#include "bfs.h"
#include "graph.h"
#include "nde.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        errx(1, "missing file name");
    }
    auto graph = nde::load(argv[1]);

    BFS bfs(graph.order());
    bfs(graph, 0);
    std::cout << "Max distance: " << bfs.max_distance << "\n";
}
