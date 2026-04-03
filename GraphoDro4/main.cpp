#include <iostream>
#include "src/graph/Graph.hpp"
#include "src/graph/generators/GraphGenerators.hpp"
#include "src/metrics/GraphMetrics.hpp"
#include "src/graph/serializers/GraphVizSerializer.hpp"
#include "src/graph/algorithms/DFS.hpp"

int main() {
    std::cout << "\n========================================\n";
    std::cout << "   GraphoDro4 - Graph Analysis Tool\n";
    std::cout << "========================================\n\n";
    
    
    // 1. Генерация
    std::cout << "[1] Generating complete graph K5...\n";
    Graph g = GraphGenerators::complete(5);
    std::cout << "    Vertices: " << g.numVertices() 
              << ", Edges: " << g.numEdges() << "\n";
    
    // 2. Метрики
    std::cout << "[2] Computing metrics...\n";
    std::cout << "    Density: " << GraphMetrics::density(g) 
              << ", Diameter: " << GraphMetrics::diameter(g) << "\n";
    
    // 3. Сериализация
    std::cout << "[3] Saving visualization...\n";
    GraphVizSerializer::toFile(g, "demo.dot");
    std::cout << "    Saved: demo.dot\n";
    
    // 4. DFS
    std::cout << "[4] Running DFS...\n";
    class SimpleVisitor : public DFSVisitor {
    public:
        size_t count = 0;
        void onVisitVertex(size_t) override { count++; }
    };
    SimpleVisitor vis;
    DFS::traverse(g, 0, vis);
    std::cout << "    Visited " << vis.count << " vertices\n";
    
    // Итог 
    std::cout << "\n========================================\n";
    std::cout << "All features working!\n";
    std::cout << "\nFor interactive mode: cli.exe\n";
    std::cout << "For automated tests: test_*.exe\n";
    std::cout << "========================================\n\n";
    
    return 0;
}