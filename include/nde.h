/*
 * Node Degree Edge format
 */

#pragma once

#include <filesystem>

#include "graph.h"

namespace fs = std::filesystem;

namespace nde {

Graph load(fs::path filename);

void serialize(const Graph& graph, fs::path filename);

};
