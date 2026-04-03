#include "AdjacencyMatrixParser.hpp"

Graph AdjacencyMatrixParser::parse(const std::string& data) {
    Graph graph;
    std::istringstream stream(data);
    std::string line;
    std::vector<std::vector<double>> matrix;

    while (std::getline(stream, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream lineStream(line);
        std::vector<double> row;
        double value;
        
        while (lineStream >> value) {
            row.push_back(value);
        }
        
        if (!row.empty()) {
            matrix.push_back(row);
        }
    }
    
    size_t n = matrix.size();
    for (size_t i = 0; i < n; i++) {
        graph.addVertex();
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] != 0 && i <= j) {
                graph.addEdge(i, j, matrix[i][j]);
            }
        }
    }
    
    return graph;
}