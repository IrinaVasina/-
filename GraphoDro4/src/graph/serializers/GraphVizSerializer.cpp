#include "GraphVizSerializer.hpp"
#include <sstream>
#include <queue>
#include <random>
#include <set>
#include <fstream> 

std::string GraphVizSerializer::serialize(const Graph& graph,
                                         const std::map<size_t, VertexStyle>& vertexStyles,
                                         const std::map<std::pair<size_t, size_t>, EdgeStyle>& edgeStyles) {
    std::ostringstream oss;
    
    oss << (graph.isDirected() ? "digraph" : "graph") << " G {\n";
    oss << "    rankdir=LR;\n";
    oss << "    node [shape=circle];\n";
    oss << "    edge [fontsize=10];\n";

    for (size_t v : graph.vertices()) {
        oss << "    " << v;

        auto it = vertexStyles.find(v);
        if (it != vertexStyles.end()) {
            oss << " [";
            oss << "color=\"" << it->second.color << "\" ";
            oss << "fillcolor=\"" << it->second.fillColor << "\" ";
            oss << "shape=\"" << it->second.shape << "\" ";
            if (!it->second.label.empty()) {
                oss << "label=\"" << it->second.label << "\" ";
            }
            oss << "style=filled";
            oss << "]";
        }
        oss << ";\n";
    }

    for (size_t v : graph.vertices()) {
        for (size_t u : graph.neighbors(v)) {
            if (v < u || graph.isDirected()) {
                oss << "    " << v << " " 
                    << (graph.isDirected() ? "-> " : "-- ") 
                    << u;

                std::pair<size_t, size_t> edgeKey = {std::min(v,u), std::max(v,u)};
                auto it = edgeStyles.find(edgeKey);
                if (it != edgeStyles.end()) {
                    oss << " [";
                    oss << "color=\"" << it->second.color << "\" ";
                    oss << "style=\"" << it->second.style << "\" ";
                    oss << "penwidth=" << it->second.penwidth << " ";
                    if (!it->second.label.empty()) {
                        oss << "label=\"" << it->second.label << "\" ";
                    }
                    oss << "]";
                }
                oss << ";\n";
            }
        }
    }
    
    oss << "}\n";
    return oss.str();
}

void GraphVizSerializer::toFile(const Graph& graph, const std::string& filename,
                               const std::map<size_t, VertexStyle>& vertexStyles,
                               const std::map<std::pair<size_t, size_t>, EdgeStyle>& edgeStyles) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    file << serialize(graph, vertexStyles, edgeStyles);
    file.close();
}

std::string GraphVizSerializer::serializeSpanningTree(const Graph& graph) {
    if (graph.numVertices() == 0) {
        return serialize(graph);
    }
    
    std::map<size_t, VertexStyle> vertexStyles;
    std::map<std::pair<size_t, size_t>, EdgeStyle> edgeStyles;

    for (size_t v : graph.vertices()) {
        VertexStyle style;
        style.fillColor = "lightgray";
        vertexStyles[v] = style;
    }

    std::vector<bool> visited(graph.numVertices(), false);
    std::queue<size_t> q;
    q.push(0);
    visited[0] = true;
    
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        
        for (size_t u : graph.neighbors(v)) {
            if (!visited[u]) {
                visited[u] = true;

                EdgeStyle style;
                style.color = "green";
                style.penwidth = 3.0;
                edgeStyles[{std::min(v,u), std::max(v,u)}] = style;
                
                q.push(u);
            }
        }
    }

    for (size_t v : graph.vertices()) {
        for (size_t u : graph.neighbors(v)) {
            if (v < u) {
                auto it = edgeStyles.find({v, u});
                if (it == edgeStyles.end()) {
                    EdgeStyle style;
                    style.color = "gray";
                    style.penwidth = 0.5;
                    style.style = "dashed";
                    edgeStyles[{v, u}] = style;
                }
            }
        }
    }
    
    return serialize(graph, vertexStyles, edgeStyles);
}

std::string GraphVizSerializer::serializeWithCycle(const Graph& graph) {
    if (graph.numVertices() == 0) {
        return serialize(graph);
    }
    
    std::map<size_t, VertexStyle> vertexStyles;
    std::map<std::pair<size_t, size_t>, EdgeStyle> edgeStyles;

    for (size_t v : graph.vertices()) {
        VertexStyle style;
        style.fillColor = "white";
        vertexStyles[v] = style;
    }

    std::vector<size_t> cycle;
    std::vector<bool> visited(graph.numVertices(), false);

    size_t start = 0;
    size_t current = start;
    size_t prev = graph.numVertices();

    std::vector<size_t> path;
    path.push_back(start);
    
    while (!path.empty()) {
        current = path.back();
        bool found = false;
        
        for (size_t neighbor : graph.neighbors(current)) {
            if (neighbor == start && path.size() > 2) {
                cycle = path;
                break;
            }
            
            if (!visited[neighbor] && neighbor != prev) {
                visited[current] = true;
                path.push_back(neighbor);
                prev = current;
                found = true;
                break;
            }
        }
        
        if (!cycle.empty()) break;
        
        if (!found) {
            path.pop_back();
        }
    }

    if (!cycle.empty()) {
        for (size_t i = 0; i < cycle.size(); i++) {
            size_t v = cycle[i];
            size_t u = cycle[(i + 1) % cycle.size()];

            VertexStyle vStyle;
            vStyle.fillColor = "yellow";
            vertexStyles[v] = vStyle;

            EdgeStyle eStyle;
            eStyle.color = "red";
            eStyle.penwidth = 3.0;
            edgeStyles[{std::min(v,u), std::max(v,u)}] = eStyle;
        }
    }

    for (size_t v : graph.vertices()) {
        for (size_t u : graph.neighbors(v)) {
            if (v < u && edgeStyles.find({v, u}) == edgeStyles.end()) {
                EdgeStyle style;
                style.color = "black";
                style.penwidth = 1.0;
                edgeStyles[{v, u}] = style;
            }
        }
    }
    
    return serialize(graph, vertexStyles, edgeStyles);
}    

void GraphVizSerializer::saveStringToFile(const std::string& content, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    file << content;
    file.close();
}
