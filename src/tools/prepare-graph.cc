/*
 * Prepare graph: isolate biggest connected components and rebuild graph from it
 */

#include <cassert>
#include <deque>
#include <err.h>
#include <iostream>
#include <vector>

#include "bfs.h"
#include "graph.h"
#include "nde.h"

template <typename Container>
std::size_t find(Container&& components, std::size_t v)
{
    size_t                   cur = v;
    std::vector<std::size_t> queue;
    while (components[cur] > 0) {
        queue.push_back(cur);
        cur = components[cur];
    }
    for (auto&& x : queue) {
        components[x] = cur;
    }
    return cur;
}

template <typename Container>
void unify(Container&& components, std::size_t v0, std::size_t v1)
{
    auto r0 = find(components, v0);
    auto r1 = find(components, v1);
    if (r0 != r1) {
        if (components[r1] < components[r0]) {
            components[r0] = r1;
            return;
        }
        if (components[r0] == components[r1]) {
            --components[r0];
        }
        components[r1] = r0;
    }
}

struct BuildComponents
{
    BuildComponents(std::size_t order) noexcept : bfs(order) {}

    auto operator()(const Graph& graph)
    {
        for (std::size_t v = 0; v < graph.order(); ++v) {
            if (not bfs.closed[v]) {
                bfs.visited_vertices = 0;
                bfs(graph, v);
                components.emplace_back(v, bfs.visited_vertices);
            }
        }
        auto&& best = components.front();
        for (auto&& comp : components) {
            if (comp.second > best.second) {
                best = comp;
            }
        }
        return best;
    }

    BFS                                              bfs;
    std::vector<std::pair<std::size_t, std::size_t>> components;
};

/*
 * For renumbering we have to rebuild a dedicated BFS as we need the numbering part
 */

static Graph renumbering(const Graph& graph, std::pair<std::size_t, std::size_t> head)
{
    const auto               sentinel = head.second + 1;
    std::deque<std::size_t>  queue;
    std::vector<std::size_t> id;
    id.resize(graph.order(), sentinel);

    std::size_t count = 0;
    queue.push_back(head.first);
    id[head.first] = 0;
    while (not queue.empty()) {
        auto&& v = queue.front();
        queue.pop_front();
        for (auto&& s : graph[v]) {
            if (id[s] == sentinel) {
                id[s] = ++count;
                queue.push_back(s);
            }
        }
    }
    Graph new_graph(head.second);
    for (std::size_t v = 0; v < graph.order(); ++v) {
        if (id[v] < sentinel) {
            for (auto&& s : graph[v]) {
                assert(id[s] < sentinel);
                if (v < s) {
                    new_graph.add_edge(id[v], id[s]);
                }
            }
        }
    }
    return new_graph;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        errx(1, "missing file name");
    }
    auto            graph = nde::load(argv[1]);
    BuildComponents bc(graph.order());
    auto            ng = renumbering(graph, bc(graph));
    nde::serialize(ng, std::cout);
}
