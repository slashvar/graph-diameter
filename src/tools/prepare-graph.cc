/*
 * Prepare graph: isolate biggest connected components and rebuild graph from it
 */

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <exception>
#include <iostream>
#include <span>
#include <utility>
#include <vector>

#include "bfs.h"
#include "graph.h"
#include "nde.h"

namespace {

struct Component
{
    std::size_t root;
    std::size_t size;
};

class BuildComponents
{
public:
    explicit BuildComponents(std::size_t order) : bfs_(order) {}

    Component operator()(const Graph& graph)
    {
        std::size_t prev_total = 0;
        for (std::size_t v = 0; v < graph.order(); ++v) {
            if (not bfs_.was_closed(v)) {
                bfs_(graph, v);
                const auto run_size = bfs_.visited() - prev_total;
                prev_total          = bfs_.visited();
                components_.emplace_back(v, run_size);
            }
        }
        auto best = components_.front();
        for (const auto& comp : components_) {
            if (comp.size > best.size) {
                best = comp;
            }
        }
        return best;
    }

private:
    BFS                    bfs_;
    std::vector<Component> components_;
};

// Rebuild the graph by renumbering vertices in BFS order from the given root.
// The returned graph has order == component.size with vertices labeled 0..size-1.
Graph renumbering(const Graph& graph, Component component)
{
    const auto               sentinel = component.size + 1;
    std::deque<std::size_t>  queue;
    std::vector<std::size_t> id(graph.order(), sentinel);

    std::size_t count = 0;
    queue.push_back(component.root);
    id[component.root] = 0;
    while (not queue.empty()) {
        const auto v = queue.front();
        queue.pop_front();
        for (auto s : graph[v]) {
            if (id[s] == sentinel) {
                id[s] = ++count;
                queue.push_back(s);
            }
        }
    }
    Graph new_graph(component.size);
    for (std::size_t v = 0; v < graph.order(); ++v) {
        if (id[v] < sentinel) {
            for (auto s : graph[v]) {
                assert(id[s] < sentinel);
                if (v < s) {
                    new_graph.add_edge(id[v], id[s]);
                }
            }
        }
    }
    return new_graph;
}

};  // namespace

int main(int argc, char* argv[])
try {
    const std::span args(argv, static_cast<std::size_t>(argc));
    if (args.size() < 2) {
        std::cerr << "missing file name\n";
        return EXIT_FAILURE;
    }
    const auto      graph = nde::load(args[1]);
    BuildComponents bc(graph.order());
    const auto      new_graph = renumbering(graph, bc(graph));
    nde::serialize(new_graph, std::cout);
    return EXIT_SUCCESS;
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
}
