#pragma once

#include "../Graph.hpp"
#include <vector>
#include <functional>
#include <iostream>

class DFSVisitor {
public:
    virtual ~DFSVisitor() = default;
    
    virtual void onVisitVertex(size_t vertex) {}

    virtual void onExploreEdge(size_t from, size_t to) {}

    virtual void onFinishVertex(size_t vertex) {}
};

class DFS {
public:
    static void traverse(const Graph& graph, size_t start, DFSVisitor& visitor);

    static void traverseAll(const Graph& graph, DFSVisitor& visitor);

    static std::vector<size_t> findPath(const Graph& graph, size_t start, size_t end);

    static std::vector<std::vector<size_t>> findConnectedComponents(const Graph& graph);
};

class PrintDFSVisitor : public DFSVisitor {
public:
    void onVisitVertex(size_t vertex) override {
        std::cout << "Visiting: " << vertex << std::endl;
    }
    
    void onExploreEdge(size_t from, size_t to) override {
        std::cout << "Exploring edge: " << from << " -> " << to << std::endl;
    }
};