#pragma once

#include "../Graph.hpp"
#include <random>
#include <vector>

class GraphGenerators {
public:
    // 1. Полный граф Kn
    static Graph complete(size_t n);
    
    // 2. Полный двудольный граф Kn,m
    static Graph completeBipartite(size_t n, size_t m);
    
    // 3. Дерево Tn
    static Graph tree(size_t n, unsigned seed = 42);
    
    // 4. Звезда Sn
    static Graph star(size_t n);
    
    // 5. Цикл Cn
    static Graph cycle(size_t n);
    
    // 6. Путь Pn
    static Graph path(size_t n);
    
    // 7. Колесо Wn
    static Graph wheel(size_t n);
    
    // 8. Случайный граф G(n, p)
    static Graph randomGraph(size_t n, double p, unsigned seed = 42);
    
    // 9. Кубический граф (3-регулярный)
    static Graph cubic(size_t n, unsigned seed = 42);
    
    // 10. Граф с заданным числом компонент связности
    static Graph withComponents(size_t n, size_t numComponents, unsigned seed = 42);
    
    // 11. Граф с заданным числом мостов
    static Graph withBridges(size_t n, size_t numBridges, unsigned seed = 42);
    
    // 12. Граф с заданным числом точек сочленения
    static Graph withArticulationPoints(size_t n, size_t numArticulations, unsigned seed = 42);
    
    // 13. Граф с заданным числом 2-мостов
    static Graph withTwoBridges(size_t n, size_t numTwoBridges, unsigned seed = 42);
    
    // 14. Граф Халина
    static Graph halin(size_t n, unsigned seed = 42);
};