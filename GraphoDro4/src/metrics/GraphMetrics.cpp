#include "GraphMetrics.hpp"
#include <queue>
#include <algorithm>
#include <limits>

static std::map<size_t, size_t> bfsDistances(const Graph& graph, size_t start) {
    std::map<size_t, size_t> distances;
    std::queue<size_t> q;
    
    distances[start] = 0;
    q.push(start);
    
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        
        for (size_t u : graph.neighbors(v)) {
            if (distances.find(u) == distances.end()) {
                distances[u] = distances[v] + 1;
                q.push(u);
            }
        }
    }
    
    return distances;
}

// 1. Плотность графа
double GraphMetrics::density(const Graph& graph) {
    size_t n = graph.numVertices();
    size_t m = graph.numEdges();
    
    if (n <= 1) return 0.0;
    
    double maxEdges = n * (n - 1) / 2.0;
    return m / maxEdges;
}

// 2. Диаметр графа
size_t GraphMetrics::diameter(const Graph& graph) {
    size_t maxDist = 0;
    
    for (size_t v : graph.vertices()) {
        auto distances = bfsDistances(graph, v);
        for (const auto& [vertex, dist] : distances) {
            if (dist != std::numeric_limits<size_t>::max()) {
                maxDist = std::max(maxDist, dist);
            }
        }
    }
    
    return maxDist;
}

// 3. Транзитивность
double GraphMetrics::transitivity(const Graph& graph) {
    size_t triangles = 0;
    size_t triples = 0;
    
    for (size_t v : graph.vertices()) {
        auto neighbors = graph.neighbors(v);
        size_t k = neighbors.size();
        
        if (k < 2) continue;
        
        triples += k * (k - 1) / 2;
        
        std::vector<size_t> neighborList(neighbors.begin(), neighbors.end());
        for (size_t i = 0; i < neighborList.size(); i++) {
            for (size_t j = i + 1; j < neighborList.size(); j++) {
                if (graph.hasEdge(neighborList[i], neighborList[j])) {
                    triangles++;
                }
            }
        }
    }
    
    return triples > 0 ? (double)triangles / triples : 0.0;
}

// 4. Количество компонент связности
size_t GraphMetrics::connectedComponents(const Graph& graph) {
    size_t n = graph.numVertices();
    std::vector<bool> visited(n, false);
    size_t components = 0;
    
    for (size_t start = 0; start < n; start++) {
        if (!visited[start]) {
            components++;
            
            std::queue<size_t> q;
            q.push(start);
            visited[start] = true;
            
            while (!q.empty()) {
                size_t v = q.front();
                q.pop();
                
                for (size_t u : graph.neighbors(v)) {
                    if (!visited[u]) {
                        visited[u] = true;
                        q.push(u);
                    }
                }
            }
        }
    }
    
    return components;
}

// 5. Точки сочленения
size_t GraphMetrics::articulationPoints(const Graph& graph) {
    size_t n = graph.numVertices();
    if (n == 0) return 0;
    
    size_t articulationCount = 0;
    
    for (size_t v = 0; v < n; v++) {
        Graph temp = graph;
        
        auto neighbors = graph.neighbors(v);
        std::vector<size_t> neighborList(neighbors.begin(), neighbors.end());

        for (size_t u : neighborList) {
            temp.removeEdge(v, u);
        }
        
        if (neighborList.size() > 1) {
            bool connected = true;

            for (size_t i = 0; i < neighborList.size() - 1 && connected; i++) {
                for (size_t j = i + 1; j < neighborList.size() && connected; j++) {
                    size_t neighbor1 = neighborList[i];
                    size_t neighbor2 = neighborList[j];
                    
                    std::vector<bool> visited(n, false);
                    std::queue<size_t> queue;
                    queue.push(neighbor1);
                    visited[neighbor1] = true;
                    
                    bool found = false;
                    while (!queue.empty() && !found) {
                        size_t current = queue.front();
                        queue.pop();
                        
                        if (current == neighbor2) {
                            found = true;
                            break;
                        }
                        
                        for (size_t next : temp.neighbors(current)) {
                            if (!visited[next] && next != v) {
                                visited[next] = true;
                                queue.push(next);
                            }
                        }
                    }
                    
                    if (!found) {
                        connected = false;
                    }
                }
            }
            
            if (!connected) {
                articulationCount++;
            }
        }
    }
    
    return articulationCount;
}

// 6. Мосты
size_t GraphMetrics::bridges(const Graph& graph) {
    size_t bridgeCount = 0;
    
    for (size_t v = 0; v < graph.numVertices(); v++) {
        auto neighbors = graph.neighbors(v);
        
        for (size_t u : neighbors) {
            if (v < u || graph.isDirected()) {
                Graph temp = graph;
                temp.removeEdge(v, u);
                
                size_t origComponents = connectedComponents(graph);
                size_t newComponents = connectedComponents(temp);
                
                if (newComponents > origComponents) {
                    bridgeCount++;
                }
            }
        }
    }
    
    return bridgeCount;
}
// 7. Проверка на двудольность
bool GraphMetrics::isBipartite(const Graph& graph) {
    size_t n = graph.numVertices();
    std::vector<int> color(n, -1);  // -1 = не окрашена, 0 и 1 = цвета
    
    for (size_t start = 0; start < n; start++) {
        if (color[start] == -1) {
            std::queue<size_t> q;
            q.push(start);
            color[start] = 0;
            
            while (!q.empty()) {
                size_t v = q.front();
                q.pop();
                
                for (size_t u : graph.neighbors(v)) {
                    if (color[u] == -1) {
                        color[u] = 1 - color[v];
                        q.push(u);
                    } else if (color[u] == color[v]) {
                        return false;
                    }
                }
            }
        }
    }
    
    return true;
}

// 8. Хроматическое число
size_t GraphMetrics::chromaticNumber(const Graph& graph) {
    size_t n = graph.numVertices();
    std::map<size_t, size_t> vertexColor;
    
    for (size_t v = 0; v < n; v++) {
        std::set<size_t> neighborColors;
        for (size_t u : graph.neighbors(v)) {
            if (vertexColor.find(u) != vertexColor.end()) {
                neighborColors.insert(vertexColor[u]);
            }
        }
        
        size_t c = 0;
        while (neighborColors.count(c) > 0) {
            c++;
        }
        
        vertexColor[v] = c;
    }
    
    size_t maxColor = 0;
    for (const auto& [vertex, color] : vertexColor) {
        maxColor = std::max(maxColor, color);
    }
    
    return maxColor + 1;
}