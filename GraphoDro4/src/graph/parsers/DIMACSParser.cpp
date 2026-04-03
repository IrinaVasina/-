#include "DIMACSParser.hpp"

Graph DIMACSParser::parse(const std::string& data) {
    Graph graph;
    std::istringstream stream(data);
    std::string line;
    size_t numVertices = 0;
    
    while (std::getline(stream, line)) {
        if (line.empty()) continue;

        if (line[0] == 'c') continue;

        if (line[0] == 'p') {
            std::istringstream lineStream(line);
            std::string p_word, edge_word;
            lineStream >> p_word >> edge_word >> numVertices;
            
            for (size_t i = 0; i < numVertices; i++) {
                graph.addVertex();
            }
            continue;
        }
        
        if (line[0] == 'e') {
            std::istringstream lineStream(line);
            std::string e_word;
            size_t from, to;
            lineStream >> e_word >> from >> to;
            
            if (from > 0 && to > 0) {
                graph.addEdge(from - 1, to - 1);
            }
        }
    }
    
    return graph;
}