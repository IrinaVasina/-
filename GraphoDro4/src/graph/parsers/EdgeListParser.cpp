#include "EdgeListParser.hpp"

Graph EdgeListParser::parse(const std::string& data) {
    Graph graph;
    std::istringstream stream(data);
    std::string line;
    
    while (std::getline(stream, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream lineStream(line);
        size_t from, to;
        double weight = 1.0;

        if (lineStream >> from >> to) {
            lineStream >> weight;
            
            while (graph.numVertices() <= std::max(from, to)) {
                graph.addVertex();
            }

            graph.addEdge(from, to, weight);
        }
    }
    
    return graph;
}