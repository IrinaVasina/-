#include "GraphGenerators.hpp"
#include <stdexcept>
#include <algorithm>

// 1. Полный граф Kn
Graph GraphGenerators::complete(size_t n) {
    Graph graph(n, false);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            graph.addEdge(i, j);
        }
    }
    return graph;
}

// 2. Полный двудольный граф Kn,m
Graph GraphGenerators::completeBipartite(size_t n, size_t m) {
    Graph graph(n + m, false);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = n; j < n + m; j++) {
            graph.addEdge(i, j);
        }
    }
    return graph;
}

// 3. Дерево Tn
Graph GraphGenerators::tree(size_t n, unsigned seed) {
    Graph graph(n, false);
    std::mt19937 rng(seed);
    for (size_t i = 1; i < n; i++) {
        std::uniform_int_distribution<size_t> dist(0, i - 1);
        size_t parent = dist(rng);
        graph.addEdge(parent, i);
    }
    return graph;
}

// 4. Звезда Sn
Graph GraphGenerators::star(size_t n) {
    Graph graph(n, false);
    for (size_t i = 1; i < n; i++) {
        graph.addEdge(0, i);
    }
    return graph;
}

// 5. Цикл Cn
Graph GraphGenerators::cycle(size_t n) {
    Graph graph(n, false);
    for (size_t i = 0; i < n; i++) {
        graph.addEdge(i, (i + 1) % n);
    }
    return graph;
}

// 6. Путь Pn
Graph GraphGenerators::path(size_t n) {
    Graph graph(n, false);
    for (size_t i = 0; i < n - 1; i++) {
        graph.addEdge(i, i + 1);
    }
    return graph;
}

// 7. Колесо Wn
Graph GraphGenerators::wheel(size_t n) {
    if (n < 4) {
        throw std::invalid_argument("Wheel graph requires at least 4 vertices");
    }
    
    Graph graph(n, false);
    
    for (size_t i = 1; i < n; i++) {
        graph.addEdge(0, i);
    }
    
    for (size_t i = 1; i < n - 1; i++) {
        graph.addEdge(i, i + 1);
    }
    graph.addEdge(1, n - 1);
    
    return graph;
}

// 8. Случайный граф G(n, p)
Graph GraphGenerators::randomGraph(size_t n, double p, unsigned seed) {
    Graph graph(n, false);
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (dist(rng) < p) {
                graph.addEdge(i, j);
            }
        }
    }
    return graph;
}

// 9. Кубический граф (3-регулярный)
Graph GraphGenerators::cubic(size_t n, unsigned seed) {
    if (n < 4 || n % 2 != 0) {
        throw std::invalid_argument("Cubic graph requires even n >= 4");
    }
    
    Graph graph(n, false);
    
    for (size_t i = 0; i < n; i++) {
        graph.addEdge(i, (i + 1) % n);
    }
    
    for (size_t i = 0; i < n / 2; i++) {
        size_t j = i + n / 2;
        if (!graph.hasEdge(i, j)) {
            graph.addEdge(i, j);
        }
    }
    
    return graph;
}
// 10. Граф с заданным числом компонент связности
Graph GraphGenerators::withComponents(size_t n, size_t numComponents, unsigned seed) {
    if (numComponents > n) {
        numComponents = n;
    }
    
    Graph graph(n, false);
    std::mt19937 rng(seed);
    
    size_t verticesPerComp = n / numComponents;
    size_t remaining = n % numComponents;
    size_t currentVertex = 0;
    
    for (size_t comp = 0; comp < numComponents; comp++) {
        size_t compSize = verticesPerComp + (comp < remaining ? 1 : 0);
        if (compSize == 0) continue;
        
        for (size_t i = 1; i < compSize; i++) {
            std::uniform_int_distribution<size_t> dist(0, i - 1);
            size_t parent = dist(rng);
            graph.addEdge(currentVertex + parent, currentVertex + i);
        }
        
        currentVertex += compSize;
    }
    
    return graph;
}

// 11. Граф с заданным числом мостов
Graph GraphGenerators::withBridges(size_t n, size_t numBridges, unsigned seed) {
    if (numBridges >= n) {
        numBridges = n - 1;
    }
    
    Graph graph(n, false);
    std::mt19937 rng(seed);
    
    for (size_t i = 0; i < numBridges; i++) {
        graph.addEdge(i, i + 1);
    }
    std::uniform_int_distribution<size_t> dist(0, n - 1);
    size_t extraEdges = (n - numBridges) * 2;
    
    for (size_t i = 0; i < extraEdges; i++) {
        size_t v1 = dist(rng);
        size_t v2 = dist(rng);
        if (v1 != v2 && !graph.hasEdge(v1, v2)) {
            graph.addEdge(v1, v2);
        }
    }
    
    return graph;
}

// 12. Граф с заданным числом точек сочленения
Graph GraphGenerators::withArticulationPoints(size_t n, size_t numArticulations, unsigned seed) {
    if (numArticulations >= n) {
        numArticulations = n - 2;
    }
    
    Graph graph(n, false);
    
    size_t verticesPerComp = n / (numArticulations + 1);
    size_t currentVertex = 0;
    
    for (size_t i = 0; i < numArticulations && currentVertex < n; i++) {
        size_t articulation = currentVertex;

        for (size_t j = 1; j < verticesPerComp && currentVertex + j < n; j++) {
            graph.addEdge(articulation, currentVertex + j);
        }

        if (i > 0) {
            graph.addEdge(articulation, articulation - 1);
        }
        
        currentVertex += verticesPerComp;
    }
    
    return graph;
}

// 13. Граф с заданным числом 2-мостов
Graph GraphGenerators::withTwoBridges(size_t n, size_t numTwoBridges, unsigned seed) {
    if (numTwoBridges * 3 > n) {
        numTwoBridges = n / 3;
    }
    
    Graph graph(n, false);
    std::mt19937 rng(seed);
    
    size_t currentVertex = 0;
    
    for (size_t i = 0; i < numTwoBridges && currentVertex + 3 < n; i++) {
        graph.addEdge(currentVertex, currentVertex + 1);
        graph.addEdge(currentVertex + 1, currentVertex + 2);
        graph.addEdge(currentVertex + 2, currentVertex + 3);
        
        currentVertex += 3;
    }

    while (currentVertex < n) {
        if (currentVertex > 0) {
            graph.addEdge(currentVertex - 1, currentVertex);
        }
        currentVertex++;
    }
    
    return graph;
}

// 14. Граф Халина
Graph GraphGenerators::halin(size_t n, unsigned seed) {
    if (n < 4) {
        throw std::invalid_argument("Halin graph requires at least 4 vertices");
    }
    
    Graph graph(n, false);
    std::mt19937 rng(seed);
    
    
    size_t numInternalVertices = n / 3;
    std::vector<size_t> leaves;

    for (size_t i = 1; i < numInternalVertices; i++) {
        std::uniform_int_distribution<size_t> dist(0, i - 1);
        size_t parent = dist(rng);
        graph.addEdge(parent, i);
    }

    for (size_t i = numInternalVertices; i < n; i++) {
        std::uniform_int_distribution<size_t> dist(0, numInternalVertices - 1);
        size_t parent = dist(rng);
        graph.addEdge(parent, i);
        leaves.push_back(i);
    }

    if (leaves.size() >= 3) {
        for (size_t i = 0; i < leaves.size(); i++) {
            size_t j = (i + 1) % leaves.size();
            graph.addEdge(leaves[i], leaves[j]);
        }
    }
    
    return graph;
}