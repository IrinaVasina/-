#include "DFS.hpp"
#include <stack>
#include <iostream>
#include <algorithm>

void DFS::traverse(const Graph& graph, size_t start, DFSVisitor& visitor) {
    if (start >= graph.numVertices()) {
        throw std::invalid_argument("Start vertex does not exist");
    }
    
    std::vector<bool> visited(graph.numVertices(), false);
    std::stack<size_t> stk;
    
    stk.push(start);
    visited[start] = true;
    
    while (!stk.empty()) {
        size_t v = stk.top();
        stk.pop();
        
        visitor.onVisitVertex(v);
        
        for (size_t u : graph.neighbors(v)) {
            visitor.onExploreEdge(v, u);
            if (!visited[u]) {
                visited[u] = true;
                stk.push(u);
            }
        }
        
        visitor.onFinishVertex(v);
    }
}

void DFS::traverseAll(const Graph& graph, DFSVisitor& visitor) {
    std::vector<bool> visited(graph.numVertices(), false);
    
    for (size_t v = 0; v < graph.numVertices(); v++) {
        if (!visited[v]) {
            std::stack<size_t> stk;
            stk.push(v);
            visited[v] = true;
            
            while (!stk.empty()) {
                size_t node = stk.top();
                stk.pop();
                
                visitor.onVisitVertex(node);
                
                for (size_t u : graph.neighbors(node)) {
                    visitor.onExploreEdge(node, u);
                    if (!visited[u]) {
                        visited[u] = true;
                        stk.push(u);
                    }
                }
                
                visitor.onFinishVertex(node);
            }
        }
    }
}

std::vector<size_t> DFS::findPath(const Graph& graph, size_t start, size_t end) {
    if (start >= graph.numVertices() || end >= graph.numVertices()) {
        throw std::invalid_argument("Vertex does not exist");
    }
    
    std::vector<bool> visited(graph.numVertices(), false);
    std::vector<size_t> parent(graph.numVertices(), graph.numVertices());
    std::stack<size_t> stk;
    
    stk.push(start);
    visited[start] = true;
    
    while (!stk.empty()) {
        size_t v = stk.top();
        stk.pop();
        
        if (v == end) break;
        
        for (size_t u : graph.neighbors(v)) {
            if (!visited[u]) {
                visited[u] = true;
                parent[u] = v;
                stk.push(u);
            }
        }
    }
    
    std::vector<size_t> path;
    for (size_t v = end; v != graph.numVertices(); v = parent[v]) {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());
    
    if (path.empty() || path[0] != start) {
        return {};
    }
    
    return path;
}

std::vector<std::vector<size_t>> DFS::findConnectedComponents(const Graph& graph) {
    std::vector<std::vector<size_t>> components;
    std::vector<bool> visited(graph.numVertices(), false);
    
    for (size_t start = 0; start < graph.numVertices(); start++) {
        if (visited[start]) continue;
        
        std::vector<size_t> component;
        std::stack<size_t> stk;
        
        stk.push(start);
        visited[start] = true;
        
        while (!stk.empty()) {
            size_t v = stk.top();
            stk.pop();
            
            component.push_back(v);
            
            for (size_t u : graph.neighbors(v)) {
                if (!visited[u]) {
                    visited[u] = true;
                    stk.push(u);
                }
            }
        }
        
        components.push_back(component);
    }
    
    return components;
}