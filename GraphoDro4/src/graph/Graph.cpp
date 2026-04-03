#include "Graph.hpp"
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>

Graph::Graph() 
    : numVertices_(0), directed_(false) {
}

Graph::Graph(size_t numVertices) 
    : numVertices_(0), directed_(false) {
    for (size_t i = 0; i < numVertices; i++) {
        addVertex();
    }
}

Graph::Graph(size_t numVertices, bool directed) 
    : numVertices_(0), directed_(directed) {
    for (size_t i = 0; i < numVertices; i++) {
        addVertex();
    }
}

Graph::Graph(bool directed) 
    : numVertices_(0), directed_(directed) {
}


size_t Graph::addVertex() {
    size_t newVertex = numVertices_;
    adjacencyList_[newVertex] = std::set<size_t>();
    numVertices_++;
    return newVertex;
}

void Graph::addEdge(size_t from, size_t to) {
    addEdge(from, to, 1.0);
}

void Graph::addEdge(size_t from, size_t to, double weight) {
    (void)weight;
    
    if (from >= numVertices_ || to >= numVertices_) {
        throw std::invalid_argument("Vertex does not exist");
    }
    
    adjacencyList_[from].insert(to);
    
    if (!directed_) {
        adjacencyList_[to].insert(from);
    }
}

bool Graph::hasEdge(size_t from, size_t to) const {
    if (from >= numVertices_ || to >= numVertices_) {
        return false;
    }
    auto it = adjacencyList_.find(from);
    if (it == adjacencyList_.end()) {
        return false;
    }
    return it->second.count(to) > 0;
}

size_t Graph::numVertices() const {
    return numVertices_;
}

size_t Graph::numEdges() const {
    size_t count = 0;
    for (const auto& [vertex, neighbors] : adjacencyList_) {
        count += neighbors.size();
    }
    return directed_ ? count : count / 2;
}

std::set<size_t> Graph::neighbors(size_t vertex) const {
    if (vertex >= numVertices_) {
        throw std::invalid_argument("Vertex does not exist");
    }
    auto it = adjacencyList_.find(vertex);
    if (it == adjacencyList_.end()) {
        return std::set<size_t>();
    }
    return it->second;
}

std::vector<size_t> Graph::vertices() const {
    std::vector<size_t> result;
    for (size_t i = 0; i < numVertices_; i++) {
        result.push_back(i);
    }
    return result;
}

void Graph::removeVertex(size_t vertex) {
    if (vertex >= numVertices_) {
        throw std::invalid_argument("Vertex does not exist");
    }
    
    for (auto& [v, neighbors] : adjacencyList_) {
        neighbors.erase(vertex);
    }
    
    adjacencyList_.erase(vertex);
    numVertices_--;
}

void Graph::removeEdge(size_t from, size_t to) {
    if (from >= numVertices_ || to >= numVertices_) {
        throw std::invalid_argument("Vertex does not exist");
    }
    
    adjacencyList_[from].erase(to);
    if (!directed_) {
        adjacencyList_[to].erase(from);
    }
}

size_t Graph::degree(size_t vertex) const {
    if (vertex >= numVertices_) {
        throw std::invalid_argument("Vertex does not exist");
    }
    auto it = adjacencyList_.find(vertex);
    if (it == adjacencyList_.end()) {
        return 0;
    }
    return it->second.size();
}

void Graph::clear() {
    numVertices_ = 0;
    adjacencyList_.clear();
}

bool Graph::isLeaf(size_t vertex) const {
    if (vertex >= numVertices_) {
        throw std::invalid_argument("Vertex does not exist");
    }
    return degree(vertex) == 1;
}

void Graph::merge(const Graph& other) {
    size_t offset = numVertices_;
    
    for (size_t i = 0; i < other.numVertices(); i++) {
        addVertex();
    }
    
    for (size_t v = 0; v < other.numVertices(); v++) {
        for (size_t u : other.neighbors(v)) {
            if (v < u || other.isDirected()) {
                addEdge(offset + v, offset + u);
            }
        }
    }
}

void Graph::renumberVertices(const std::map<size_t, size_t>& mapping) {
    if (mapping.empty()) {
        return;
    }
    
    std::map<size_t, std::set<size_t>> newAdjacencyList;

    size_t maxNewId = 0;
    for (const auto& [oldId, newId] : mapping) {
        maxNewId = std::max(maxNewId, newId);
        newAdjacencyList[newId] = std::set<size_t>();
    }
    
    for (const auto& [oldId, newId] : mapping) {
        auto it = adjacencyList_.find(oldId);
        if (it != adjacencyList_.end()) {
            for (size_t oldNeighbor : it->second) {
                auto neighborIt = mapping.find(oldNeighbor);
                if (neighborIt != mapping.end()) {
                    size_t newNeighbor = neighborIt->second;
                    newAdjacencyList[newId].insert(newNeighbor);

                    if (!directed_) {
                        newAdjacencyList[newNeighbor].insert(newId);
                    }
                }
            }
        }
    }
    
    adjacencyList_ = std::map<size_t, std::set<size_t>>(
        newAdjacencyList.begin(), 
        newAdjacencyList.end()
    );
    
    numVertices_ = maxNewId + 1;
}