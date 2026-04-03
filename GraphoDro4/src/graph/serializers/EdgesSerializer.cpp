#include "EdgesSerializer.hpp"

std::string EdgesSerializer::serialize(const Graph& graph) {
    std::string result;

    result += "# Vertices: " + std::to_string(graph.numVertices()) + "\n";
    result += "# Edges: " + std::to_string(graph.numEdges()) + "\n";
    result += "# Format: from to\n";

    for (size_t v = 0; v < graph.numVertices(); v++) {
        for (size_t u : graph.neighbors(v)) {
            if (v < u || graph.isDirected()) {
                result += std::to_string(v) + " " + std::to_string(u) + "\n";
            }
        }
    }
    
    return result;
}

void EdgesSerializer::toFile(const Graph& graph, const std::string& filename) {
    std::string content = serialize(graph);
    saveStringToFile(content, filename);
}

void EdgesSerializer::saveStringToFile(const std::string& content, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    file << content;
    file.close();
}