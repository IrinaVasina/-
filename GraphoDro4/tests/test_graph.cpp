#define CATCH_CONFIG_MAIN
#include "../external/catch.hpp"
#include "../src/graph/Graph.hpp"
#include "../src/graph/generators/GraphGenerators.hpp"
#include "../src/graph/algorithms/DFS.hpp"
#include <map>
#include <vector>
#include <stdexcept>


TEST_CASE("Graph basic operations work correctly", "[graph][basic]") {
    
    SECTION("Empty graph has zero vertices and edges") {
        Graph g;
        REQUIRE(g.numVertices() == 0);
        REQUIRE(g.numEdges() == 0);
    }
    
    SECTION("Adding vertices increases count") {
        Graph g;
        g.addVertex();
        g.addVertex();
        g.addVertex();
        
        REQUIRE(g.numVertices() == 3);
        REQUIRE(g.numEdges() == 0);
    }
    
    SECTION("Adding edges works correctly") {
        Graph g{static_cast<size_t>(4)};
        
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        
        REQUIRE(g.numEdges() == 3);
        REQUIRE(g.hasEdge(0, 1) == true);
        REQUIRE(g.hasEdge(1, 2) == true);
        REQUIRE(g.hasEdge(2, 3) == true);
        
        REQUIRE(g.hasEdge(0, 3) == false);
        REQUIRE(g.hasEdge(1, 3) == false);
    }
    
    SECTION("Degree returns correct values") {
        Graph g{static_cast<size_t>(4)};
        
        g.addEdge(0, 1);
        g.addEdge(0, 2);
        g.addEdge(0, 3);
        g.addEdge(1, 2);
        
        REQUIRE(g.degree(0) == 3);
        REQUIRE(g.degree(1) == 2);
        REQUIRE(g.degree(2) == 2);
        REQUIRE(g.degree(3) == 1);
    }
    
    SECTION("Neighbors returns correct set") {
        Graph g{static_cast<size_t>(4)};
        
        g.addEdge(0, 1);
        g.addEdge(0, 2);
        g.addEdge(0, 3);
        
        auto neighbors = g.neighbors(0);
        
        REQUIRE(neighbors.size() == 3);
        REQUIRE(neighbors.count(1) == 1);
        REQUIRE(neighbors.count(2) == 1);
        REQUIRE(neighbors.count(3) == 1);
    }
    
    SECTION("Cannot add edge to non-existent vertex") {
        Graph g{static_cast<size_t>(3)};

        REQUIRE_THROWS_AS(g.addEdge(0, 100), std::invalid_argument);
        REQUIRE_THROWS_AS(g.addEdge(100, 0), std::invalid_argument);
    }
}


TEST_CASE("isLeaf method works correctly", "[graph][isLeaf]") {
    
    SECTION("Isolated vertex is not a leaf") {
        Graph g{static_cast<size_t>(3)};
        REQUIRE(g.isLeaf(0) == false);
        REQUIRE(g.isLeaf(1) == false);
        REQUIRE(g.isLeaf(2) == false);
    }
    
    SECTION("Vertex with degree 1 is a leaf") {
        Graph g{static_cast<size_t>(3)};
        g.addEdge(0, 1);
        
        REQUIRE(g.isLeaf(0) == true);
        REQUIRE(g.isLeaf(1) == true);
        REQUIRE(g.isLeaf(2) == false);
    }
    
    SECTION("Vertex with degree > 1 is not a leaf") {
        Graph g{static_cast<size_t>(4)};
        g.addEdge(0, 1);
        g.addEdge(0, 2);
        g.addEdge(0, 3);
        
        REQUIRE(g.isLeaf(0) == false);
        REQUIRE(g.isLeaf(1) == true);
        REQUIRE(g.isLeaf(2) == true);
        REQUIRE(g.isLeaf(3) == true);
    }
    
    SECTION("Star graph has correct leaves") {
        Graph g = GraphGenerators::star(static_cast<size_t>(6));
        
        REQUIRE(g.isLeaf(0) == false);
        
        for (size_t v = 1; v < 6; v++) {
            REQUIRE(g.isLeaf(v) == true);
        }
    }
}


TEST_CASE("merge method combines graphs correctly", "[graph][merge]") {
    
    SECTION("Merge two empty graphs") {
        Graph g1, g2;
        g1.merge(g2);
        
        REQUIRE(g1.numVertices() == 0);
        REQUIRE(g1.numEdges() == 0);
    }
    
    SECTION("Merge path and cycle") {
        Graph g1 = GraphGenerators::path(static_cast<size_t>(4));
        Graph g2 = GraphGenerators::cycle(static_cast<size_t>(3));
        
        size_t v1 = g1.numVertices();
        size_t v2 = g2.numVertices();
        size_t e1 = g1.numEdges();
        size_t e2 = g2.numEdges();
        
        g1.merge(g2);
        
        REQUIRE(g1.numVertices() == v1 + v2);
        REQUIRE(g1.numEdges() == e1 + e2);
    }
    
    SECTION("Merged graph preserves original edges") {
        Graph g1{static_cast<size_t>(3)};
        g1.addEdge(0, 1);
        g1.addEdge(1, 2);
        
        Graph g2{static_cast<size_t>(2)};
        g2.addEdge(0, 1);
        
        g1.merge(g2);
        
        REQUIRE(g1.hasEdge(0, 1) == true);
        REQUIRE(g1.hasEdge(1, 2) == true);
        REQUIRE(g1.hasEdge(3, 4) == true);
    }
}


TEST_CASE("renumberVertices remaps vertices correctly", "[graph][renumber]") {
    
    SECTION("Identity mapping preserves graph") {
        Graph g = GraphGenerators::path(static_cast<size_t>(4));
        
        std::map<size_t, size_t> identity = {{0,0}, {1,1}, {2,2}, {3,3}};
        g.renumberVertices(identity);
        
        REQUIRE(g.numVertices() == 4);
        REQUIRE(g.numEdges() == 3);
        REQUIRE(g.hasEdge(0, 1) == true);
        REQUIRE(g.hasEdge(1, 2) == true);
        REQUIRE(g.hasEdge(2, 3) == true);
    }
    
    SECTION("Reverse mapping reverses path") {
        Graph g = GraphGenerators::path(static_cast<size_t>(4));
        
        std::map<size_t, size_t> reverse = {{0,3}, {1,2}, {2,1}, {3,0}};
        g.renumberVertices(reverse);
        
        REQUIRE(g.hasEdge(3, 2) == true);
        REQUIRE(g.hasEdge(2, 1) == true);
        REQUIRE(g.hasEdge(1, 0) == true);

        REQUIRE(g.numEdges() == 3);
    }
    
    SECTION("Renumbering preserves degrees") {
        Graph g{static_cast<size_t>(4)};
        g.addEdge(0, 1);
        g.addEdge(0, 2);
        g.addEdge(0, 3);
        
        std::vector<size_t> origDegrees(4);
        for (size_t v = 0; v < 4; v++) {
            origDegrees[v] = g.degree(v);
        }
        
        std::map<size_t, size_t> mapping = {{0,3}, {1,0}, {2,1}, {3,2}};
        g.renumberVertices(mapping);
        
        REQUIRE(g.degree(3) == origDegrees[0]);
        REQUIRE(g.degree(0) == origDegrees[1]);
        REQUIRE(g.degree(1) == origDegrees[2]);
        REQUIRE(g.degree(2) == origDegrees[3]);
    }
}


TEST_CASE("Graph handles invalid input gracefully", "[graph][errors]") {
    
    SECTION("hasEdge with invalid vertex") {
        Graph g{static_cast<size_t>(3)};
        REQUIRE(g.hasEdge(0, 100) == false);
        REQUIRE(g.hasEdge(100, 0) == false);
    }
    
    SECTION("degree with invalid vertex") {
        Graph g{static_cast<size_t>(3)};
        REQUIRE_THROWS_AS(g.degree(100), std::invalid_argument);
    }
    
    SECTION("neighbors with invalid vertex") {
        Graph g{static_cast<size_t>(3)};
        REQUIRE_THROWS_AS(g.neighbors(100), std::invalid_argument);
    }
}