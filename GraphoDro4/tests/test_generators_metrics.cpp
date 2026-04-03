#define CATCH_CONFIG_MAIN
#include "../external/catch.hpp"
#include "../src/graph/Graph.hpp"
#include "../src/graph/generators/GraphGenerators.hpp"
#include "../src/graph/serializers/GraphVizSerializer.hpp"
#include "../src/graph/algorithms/DFS.hpp"  
#include "../src/metrics/GraphMetrics.hpp"
#include "../src/graph/serializers/EdgesSerializer.hpp" 
#include <cmath>
#include <fstream>


TEST_CASE("Graph generators create correct structures", "[generators]") {
    
    SECTION("Complete graph Kn") {
        Graph g = GraphGenerators::complete(5);
        
        REQUIRE(g.numVertices() == 5);
        REQUIRE(g.numEdges() == 10);
        
        for (size_t v = 0; v < 5; v++) {
            REQUIRE(g.degree(v) == 4);
        }
        
        for (size_t i = 0; i < 5; i++) {
            for (size_t j = i + 1; j < 5; j++) {
                REQUIRE(g.hasEdge(i, j));
            }
        }
    }
    
    SECTION("Cycle graph Cn") {
        Graph g = GraphGenerators::cycle(6);
        
        REQUIRE(g.numVertices() == 6);
        REQUIRE(g.numEdges() == 6);
        
        for (size_t v = 0; v < 6; v++) {
            REQUIRE(g.degree(v) == 2);
        }
    }
    
    SECTION("Path graph Pn") {
        Graph g = GraphGenerators::path(5);
        
        REQUIRE(g.numVertices() == 5);
        REQUIRE(g.numEdges() == 4);
        
        REQUIRE(g.degree(0) == 1);
        REQUIRE(g.degree(4) == 1);
        
        for (size_t v = 1; v < 4; v++) {
            REQUIRE(g.degree(v) == 2);
        }
    }
    
    SECTION("Star graph Sn") {
        Graph g = GraphGenerators::star(6);
        
        REQUIRE(g.numVertices() == 6);
        REQUIRE(g.numEdges() == 5);
        
        REQUIRE(g.degree(0) == 5);
        
        for (size_t v = 1; v < 6; v++) {
            REQUIRE(g.degree(v) == 1);
            REQUIRE(g.hasEdge(0, v));
        }
    }
    
    SECTION("Wheel graph Wn") {
        Graph g = GraphGenerators::wheel(6);
        
        REQUIRE(g.numVertices() == 6);
        REQUIRE(g.numEdges() == 10);
        
        REQUIRE(g.degree(0) == 5);
        
        for (size_t v = 1; v < 6; v++) {
            REQUIRE(g.degree(v) == 3);
        }
    }
    
    SECTION("Complete bipartite graph Kn,m") {
        Graph g = GraphGenerators::completeBipartite(3, 4);
        
        REQUIRE(g.numVertices() == 7);
        REQUIRE(g.numEdges() == 12);
        
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 3; j < 7; j++) {
                REQUIRE(g.hasEdge(i, j));
            }
        }
    }
    
    SECTION("Tree Tn") {
        Graph g = GraphGenerators::tree(10, 42);
        
        REQUIRE(g.numVertices() == 10);
        REQUIRE(g.numEdges() == 9);
        
        REQUIRE(GraphMetrics::connectedComponents(g) == 1);
    }
    
    SECTION("Random graph G(n,p)") {
        Graph g = GraphGenerators::randomGraph(20, 0.3, 42);
        
        REQUIRE(g.numVertices() == 20);
        
        size_t expectedEdges = 20 * 19 / 2 * 0.3;
        size_t actualEdges = g.numEdges();
        
        REQUIRE(actualEdges > expectedEdges * 0.5);
        REQUIRE(actualEdges < expectedEdges * 1.5);
    }
    
    SECTION("Graph with k components") {
        Graph g = GraphGenerators::withComponents(15, 3, 42);
        
        REQUIRE(g.numVertices() == 15);
        
        size_t components = GraphMetrics::connectedComponents(g);
        REQUIRE(components == 3);
    }
}


TEST_CASE("Graph metrics compute correctly", "[metrics]") {
    
    SECTION("Density of complete graph") {
        Graph g = GraphGenerators::complete(5);
        double density = GraphMetrics::density(g);
        
        REQUIRE(density == Approx(1.0).epsilon(0.01));
    }
    
    SECTION("Density of sparse graph") {
        Graph g = GraphGenerators::path(10);
        double density = GraphMetrics::density(g);
        
        REQUIRE(density == Approx(0.2).epsilon(0.01));
    }
    
    SECTION("Diameter of path") {
        Graph g = GraphGenerators::path(6);
        size_t diameter = GraphMetrics::diameter(g);
        
        REQUIRE(diameter == 5);
    }
    
    SECTION("Diameter of complete graph") {
        Graph g = GraphGenerators::complete(5);
        size_t diameter = GraphMetrics::diameter(g);
        
        REQUIRE(diameter == 1);
    }
    
    SECTION("Connected components count") {
        Graph connected = GraphGenerators::complete(5);
        REQUIRE(GraphMetrics::connectedComponents(connected) == 1);
        
        Graph disconnected = GraphGenerators::withComponents(10, 3, 42);
        REQUIRE(GraphMetrics::connectedComponents(disconnected) == 3);
    }
    
    SECTION("Bipartite check") {
        Graph bipartite1 = GraphGenerators::completeBipartite(3, 4);
        REQUIRE(GraphMetrics::isBipartite(bipartite1) == true);
        
        Graph bipartite2 = GraphGenerators::path(5);
        REQUIRE(GraphMetrics::isBipartite(bipartite2) == true);
        
        Graph bipartite3 = GraphGenerators::cycle(6);
        REQUIRE(GraphMetrics::isBipartite(bipartite3) == true);
        
        Graph notBipartite = GraphGenerators::cycle(5);
        REQUIRE(GraphMetrics::isBipartite(notBipartite) == false);
    }
    
    SECTION("Chromatic number") {
        Graph complete = GraphGenerators::complete(5);
        REQUIRE(GraphMetrics::chromaticNumber(complete) == 5);
        
        Graph bipartite = GraphGenerators::completeBipartite(3, 4);
        REQUIRE(GraphMetrics::chromaticNumber(bipartite) == 2);
    }
    
    
    SECTION("Articulation points in path graph") {
        Graph g = GraphGenerators::path(5);  // 0-1-2-3-4
        
        size_t ap = GraphMetrics::articulationPoints(g);
        REQUIRE(ap == 3);
    }
    
    SECTION("Articulation points in cycle graph") {
        Graph g = GraphGenerators::cycle(5);
        
        size_t ap = GraphMetrics::articulationPoints(g);
        REQUIRE(ap == 0);
    }
    
    SECTION("Articulation points in star graph") {
        Graph g = GraphGenerators::star(6);
        
        size_t ap = GraphMetrics::articulationPoints(g);
        REQUIRE(ap == 1);
    }
    
    SECTION("Articulation points in complete graph") {
        Graph g = GraphGenerators::complete(5);
        
        size_t ap = GraphMetrics::articulationPoints(g);
        REQUIRE(ap == 0);
    }
    
    SECTION("Bridges in path graph") {
        Graph g = GraphGenerators::path(5);
        
        size_t b = GraphMetrics::bridges(g);
        REQUIRE(b == 4);
    }
    
    SECTION("Bridges in cycle graph") {
        Graph g = GraphGenerators::cycle(5);
        
        size_t b = GraphMetrics::bridges(g);
        REQUIRE(b == 0);
    }
    
    SECTION("Bridges in star graph") {
        Graph g = GraphGenerators::star(6);
        
        size_t b = GraphMetrics::bridges(g);
        REQUIRE(b == 5);
    }
    
    SECTION("Bridges in complete graph") {
        Graph g = GraphGenerators::complete(5);
        
        size_t b = GraphMetrics::bridges(g);
        REQUIRE(b == 0);
    }
}


TEST_CASE("Graph invariants are preserved", "[invariants]") {
    
    SECTION("Spanning tree is a valid tree") {
        Graph g = GraphGenerators::complete(6);
        std::string dot = GraphVizSerializer::serializeSpanningTree(g);
        
        REQUIRE(dot.find("graph G {") != std::string::npos);
        REQUIRE(dot.find("color=\"green\"") != std::string::npos);
        
        Graph tree = GraphGenerators::path(6);
        REQUIRE(tree.numEdges() == 5);
        REQUIRE(GraphMetrics::connectedComponents(tree) == 1);
    }
    
    SECTION("Cycle has correct structure") {
        Graph g = GraphGenerators::cycle(7);
        std::string dot = GraphVizSerializer::serializeWithCycle(g);
        
        REQUIRE(dot.find("graph G {") != std::string::npos);
        
        for (size_t v = 0; v < 7; v++) {
            REQUIRE(g.degree(v) == 2);
        }
        
        REQUIRE(g.numEdges() == 7);
    }
    
    SECTION("Merged graph preserves properties") {
        Graph g1 = GraphGenerators::path(4);
        Graph g2 = GraphGenerators::cycle(3);
        
        size_t v1 = g1.numVertices();
        size_t v2 = g2.numVertices();
        size_t e1 = g1.numEdges();
        size_t e2 = g2.numEdges();
        
        g1.merge(g2);
        
        REQUIRE(g1.numVertices() == v1 + v2);
        REQUIRE(g1.numEdges() == e1 + e2);
    }
    
    SECTION("Leaf detection is correct") {
        Graph star = GraphGenerators::star(6);
        
        REQUIRE(star.isLeaf(0) == false);
        
        for (size_t v = 1; v < 6; v++) {
            REQUIRE(star.isLeaf(v) == true);
        }
    }
}


TEST_CASE("DFS algorithms work correctly", "[dfs][algorithms]") {
    
    SECTION("DFS visits all vertices in connected graph") {
        Graph g = GraphGenerators::complete(5);
        
        class CountVisitor : public DFSVisitor {
        public:
            size_t count = 0;
            void onVisitVertex(size_t vertex) override {
                count++;
            }
        };
        
        CountVisitor visitor;
        DFS::traverse(g, 0, visitor);
        
        REQUIRE(visitor.count == 5);
    }
    
    SECTION("DFS finds correct path") {
        Graph g = GraphGenerators::path(6);
        
        auto path = DFS::findPath(g, 0, 5);
        
        REQUIRE(path.size() == 6);
        REQUIRE(path[0] == 0);
        REQUIRE(path[5] == 5);
        
        for (size_t i = 0; i < 5; i++) {
            REQUIRE(path[i+1] == path[i] + 1);
        }
    }
    
    SECTION("DFS finds connected components") {
        Graph g = GraphGenerators::withComponents(12, 3, 42);
        
        auto components = DFS::findConnectedComponents(g);
        
        REQUIRE(components.size() == 3);
        
        size_t totalVertices = 0;
        for (const auto& comp : components) {
            totalVertices += comp.size();
        }
        REQUIRE(totalVertices == 12);
    }
}

TEST_CASE("EdgesSerializer works correctly", "[serializer][edges]") {
    
    SECTION("Serialize simple graph") {
        Graph g = GraphGenerators::path(4);
        std::string edges = EdgesSerializer::serialize(g);
        
        REQUIRE(edges.find("0 1") != std::string::npos);
        REQUIRE(edges.find("1 2") != std::string::npos);
        REQUIRE(edges.find("2 3") != std::string::npos);
        REQUIRE(edges.find("# Vertices: 4") != std::string::npos);
        REQUIRE(edges.find("# Edges: 3") != std::string::npos);
    }
    
    SECTION("Save to file") {
        Graph g = GraphGenerators::cycle(5);
        EdgesSerializer::toFile(g, "test_edges.edges");
        
        std::ifstream file("test_edges.edges");
        REQUIRE(file.is_open());
        
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        REQUIRE(content.find("0 1") != std::string::npos);
        REQUIRE(content.find("# Vertices: 5") != std::string::npos);
        
        file.close();
    }
}