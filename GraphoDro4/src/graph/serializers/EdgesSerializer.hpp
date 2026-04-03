#pragma once

#include "../Graph.hpp"
#include <string>
#include <fstream>

class EdgesSerializer {
public:
    static std::string serialize(const Graph& graph);

    static void toFile(const Graph& graph, const std::string& filename);

    static void saveStringToFile(const std::string& content, const std::string& filename);
};