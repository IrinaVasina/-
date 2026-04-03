#include "SNAPParser.hpp"

Graph SNAPParser::parse(const std::string& data) {
    Graph graph;
    std::istringstream stream(data);
    std::string line;
    size_t maxVertex = 0;
    std::vector<std::pair<size_t, size_t>> edges;
    
    while (std::getline(stream, line)) {
        if (line.empty()) continue;

        if (line[0] == '#') continue;

        std::istringstream lineStream(line);
        size_t from, to;

        if (lineStream >> from >> to) {
            edges.push_back({from, to});
            maxVertex = std::max(maxVertex, std::max(from, to));
        }
    }

    for (size_t i = 0; i <= maxVertex; i++) {
        graph.addVertex();
    }

    for (const auto& edge : edges) {
        graph.addEdge(edge.first, edge.second);
    }
    
    return graph;
}