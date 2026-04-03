#pragma once

#include "../graph/Graph.hpp"
#include <vector>
#include <map>

class GraphMetrics {
public:
    // 1. Плотность графа
    static double density(const Graph& graph);
    
    // 2. Диаметр графа
    static size_t diameter(const Graph& graph);
    
    // 3. Транзитивность
    static double transitivity(const Graph& graph);
    
    // 4. Количество компонент связности
    static size_t connectedComponents(const Graph& graph);
    
    // 5. Количество точек сочленения
    static size_t articulationPoints(const Graph& graph);
    
    // 6. Количество мостов
    static size_t bridges(const Graph& graph);
    
    // 7. Проверка на двудольность
    static bool isBipartite(const Graph& graph);
    
    // 8. Хроматическое число
    static size_t chromaticNumber(const Graph& graph);
};