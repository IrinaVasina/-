#include "src/graph/Graph.hpp"
#include "src/graph/generators/GraphGenerators.hpp"
#include "src/metrics/GraphMetrics.hpp"
#include "src/graph/serializers/GraphVizSerializer.hpp"
#include "src/graph/algorithms/DFS.hpp"
#include "src/graph/parsers/EdgeListParser.hpp"
#include "src/graph/parsers/AdjacencyMatrixParser.hpp"
#include "src/graph/parsers/DIMACSParser.hpp"
#include "src/graph/parsers/SNAPParser.hpp"
#include "src/graph/serializers/EdgesSerializer.hpp"
#include <iostream>
#include <string>
#include <sstream>

void printHelp() {
    std::cout << "\n=== GraphoDro4 - Graph Analysis Tool ===\n"
              << "\nCommands:\n"
              << "  generate <type> <n> [params] - Generate graph\n"
              << "    Types: complete, cycle, path, star, wheel, tree, random, bipartite, components\n"
              << "  load <format> <file>         - Load graph from file\n"
              << "    Formats: edgelist, adjmatrix, dimacs, snap\n"
              << "  save <file>                  - Save graph to DOT file\n"
              << "  metrics                      - Compute all metrics\n"
              << "  dfs <start>                  - Run DFS from vertex\n"
              << "  info                         - Show graph info\n"
              << "  help                         - Show this help\n"
              << "  exit                         - Exit\n"
              << std::endl;
}

void printInfo(const Graph& g) {
    std::cout << "\n=== Graph Info ===\n"
              << "Vertices: " << g.numVertices() << "\n"
              << "Edges: " << g.numEdges() << "\n"
              << "Directed: " << (g.isDirected() ? "yes" : "no") << std::endl;
}

void printMetrics(const Graph& g) {
    std::cout << "\n=== Graph Metrics ===\n"
              << "Density: " << GraphMetrics::density(g) << "\n"
              << "Diameter: " << GraphMetrics::diameter(g) << "\n"
              << "Connected Components: " << GraphMetrics::connectedComponents(g) << "\n"
              << "Bipartite: " << (GraphMetrics::isBipartite(g) ? "yes" : "no") << "\n"
              << "Chromatic Number: " << GraphMetrics::chromaticNumber(g) << "\n"
              << "Transitivity: " << GraphMetrics::transitivity(g) << std::endl;
}

int main() {
    Graph graph;
    std::string line;
    
    std::cout << "\n========================================\n";
    std::cout << "   GraphoDro4 - Graph Analysis Tool\n";
    std::cout << "========================================\n";
    std::cout << "Type 'help' for available commands.\n\n";
    
    while (true) {
        std::cout << "grapho> ";
        std::getline(std::cin, line);
        
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        
        if (cmd == "exit" || cmd == "quit") {
            std::cout << "Goodbye!\n";
            break;
        }
        else if (cmd == "help") {
            printHelp();
        }
        else if (cmd == "info") {
            printInfo(graph);
        }
        else if (cmd == "metrics") {
            printMetrics(graph);
        }
        else if (cmd == "generate") {
            std::string type;
            size_t n;
            iss >> type >> n;
            
            try {
                if (type == "complete") {
                    graph = GraphGenerators::complete(n);
                }
                else if (type == "cycle") {
                    graph = GraphGenerators::cycle(n);
                }
                else if (type == "path") {
                    graph = GraphGenerators::path(n);
                }
                else if (type == "star") {
                    graph = GraphGenerators::star(n);
                }
                else if (type == "wheel") {
                    graph = GraphGenerators::wheel(n);
                }
                else if (type == "tree") {
                    graph = GraphGenerators::tree(n, 42);
                }
                else if (type == "random") {
                    double p;
                    iss >> p;
                    graph = GraphGenerators::randomGraph(n, p, 42);
                }
                else if (type == "bipartite") {
                    size_t m;
                    iss >> m;
                    graph = GraphGenerators::completeBipartite(n, m);
                }
                else if (type == "components") {
                    size_t k;
                    iss >> k;
                    graph = GraphGenerators::withComponents(n, k, 42);
                }
                else {
                    std::cout << "Unknown generator type.\n";
                    continue;
                }
                std::cout << "Graph generated: " << n << " vertices, " << graph.numEdges() << " edges\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
        else if (cmd == "load") {
            std::string format, file;
            iss >> format >> file;
            
            try {
                if (format == "edgelist") {
                    EdgeListParser parser;
                    graph = parser.parse(file);
                }
                else if (format == "adjmatrix") {
                    AdjacencyMatrixParser parser;
                    graph = parser.parse(file);
                }
                else if (format == "dimacs") {
                    DIMACSParser parser;
                    graph = parser.parse(file);
                }
                else if (format == "snap") {
                    SNAPParser parser;
                    graph = parser.parse(file);
                }
                else {
                    std::cout << "Unknown format.\n";
                    continue;
                }
                std::cout << "Graph loaded from " << file << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error loading graph: " << e.what() << "\n";
            }
        }
        else if (cmd == "save") {
            std::string file;
            iss >> file;
            
            GraphVizSerializer::toFile(graph, file);
            std::cout << "Graph saved to " << file << "\n";
        }
        else if (cmd == "dfs") {
            size_t start;
            iss >> start;
            
            if (start >= graph.numVertices()) {
                std::cout << "Invalid vertex: " << start << "\n";
                continue;
            }
            
            class CompactVisitor : public DFSVisitor {
            public:
                std::vector<size_t> order;
                size_t edgeCount = 0;
                
                void onVisitVertex(size_t vertex) override {
                    order.push_back(vertex);
                }
                
                void onExploreEdge(size_t from, size_t to) override {
                    edgeCount++;
                }
                
                void printSummary() {
                    std::cout << "DFS visit order: ";
                    for (size_t v : order) {
                        std::cout << v << " ";
                    }
                    std::cout << "\nExplored " << edgeCount << " edges\n";
                }
            };
            
            CompactVisitor visitor;
            DFS::traverse(graph, start, visitor);
            visitor.printSummary();
        }
        else if (cmd.empty()) {
        }
        else if (cmd == "save-edges") {
            std::string file;
            iss >> file;
    
            EdgesSerializer::toFile(graph, file);
            std::cout << "Graph saved to " << file << " (edges format)\n";
        }
        else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }
    
    return 0;
}