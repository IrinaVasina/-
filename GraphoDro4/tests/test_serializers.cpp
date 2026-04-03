#define CATCH_CONFIG_MAIN
#include "../external/catch.hpp"
#include <fstream>
#include <filesystem>
#include "../src/graph/Graph.hpp"
#include "../src/graph/generators/GraphGenerators.hpp"
#include "../src/graph/serializers/GraphVizSerializer.hpp"

TEST_CASE("GraphVizSerializer creates valid DOT files", "[serializer]") {
    Graph graph = GraphGenerators::cycle(5);
    
    SECTION("Simple serialization") {
        GraphVizSerializer::toFile(graph, "test_output.dot");
        
REQUIRE(std::filesystem::exists("test_output.dot"));

{
    std::ifstream file("test_output.dot");
    REQUIRE(file.is_open());
    
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    
    REQUIRE(content.find("graph G {") != std::string::npos);
    REQUIRE(content.find("rankdir=LR") != std::string::npos);
    
    file.close();
}

std::error_code ec;
std::filesystem::remove("test_output.dot", ec);
    }
    
    SECTION("Spanning tree serialization") {
        Graph complete = GraphGenerators::complete(4);
        std::string dot = GraphVizSerializer::serializeSpanningTree(complete);
        
        REQUIRE(dot.find("graph G {") != std::string::npos);
        REQUIRE(dot.find("color=\"green\"") != std::string::npos);
        REQUIRE(dot.find("penwidth=3") != std::string::npos);
    }
    
    SECTION("Cycle serialization") {
        Graph complete = GraphGenerators::complete(4);
        std::string dot = GraphVizSerializer::serializeWithCycle(complete);
        
        REQUIRE(dot.find("graph G {") != std::string::npos);
        if (dot.find("fillcolor=\"yellow\"") != std::string::npos) {
            REQUIRE(dot.find("color=\"red\"") != std::string::npos);
        }
    }
}

TEST_CASE("GraphVizSerializer handles edge cases", "[serializer]") {
    SECTION("Empty graph") {
        Graph graph;
        std::string dot = GraphVizSerializer::serialize(graph);
        REQUIRE(dot.find("graph G {") != std::string::npos);
    }
    
    SECTION("Single vertex") {
        Graph graph(1, false);
        std::string dot = GraphVizSerializer::serialize(graph);
        REQUIRE(dot.find("0") != std::string::npos);
    }
}