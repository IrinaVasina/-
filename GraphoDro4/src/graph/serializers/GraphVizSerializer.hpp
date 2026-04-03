#pragma once

#include "../Graph.hpp"
#include <string>
#include <map>
#include <fstream>

class GraphVizSerializer {
public:
    struct VertexStyle {
        std::string color = "black";
        std::string fillColor = "white";
        std::string shape = "circle";
        std::string label;
    };

    struct EdgeStyle {
        std::string color = "black";
        std::string style = "solid";
        double penwidth = 1.0;
        std::string label;
    };

    static std::string serialize(const Graph& graph,
                                const std::map<size_t, VertexStyle>& vertexStyles = {},
                                const std::map<std::pair<size_t, size_t>, EdgeStyle>& edgeStyles = {});
    
    static void toFile(const Graph& graph, const std::string& filename,
                      const std::map<size_t, VertexStyle>& vertexStyles = {},
                      const std::map<std::pair<size_t, size_t>, EdgeStyle>& edgeStyles = {});

    static void saveStringToFile(const std::string& content, const std::string& filename);

    static std::string serializeSpanningTree(const Graph& graph);

    static std::string serializeWithCycle(const Graph& graph);
};