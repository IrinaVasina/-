#pragma once

#include <vector>
#include <set>
#include <map>

class Graph {
private:
    size_t numVertices_;  // Количество вершин
    std::map<size_t, std::set<size_t>> adjacencyList_;  // Список смежности
    bool directed_;  // Ориентированный ли граф

public:
    
    // 1. Пустой неориентированный граф
    Graph();
    
    // 2. Граф с n вершинами
    explicit Graph(size_t numVertices);
    
    // 3. Полный конструктор
    Graph(size_t numVertices, bool directed);
    
    // 4. Только ориентированность
    explicit Graph(bool directed);
    
    
    // Добавить вершину, возвращает её номер
    size_t addVertex();
    
    // Добавить ребро между вершинами
    void addEdge(size_t from, size_t to);
    void addEdge(size_t from, size_t to, double weight);
    
    // Проверить, есть ли ребро
    bool hasEdge(size_t from, size_t to) const;
    
    // Получить количество вершин
    size_t numVertices() const;
    
    // Получить количество рёбер
    size_t numEdges() const;
    
    // Получить соседей вершины
    std::set<size_t> neighbors(size_t vertex) const;

    // Получить все вершины
    std::vector<size_t> vertices() const;

    // Проверка на ориентированность
    bool isDirected() const { return directed_; }

    // Удалить вершину
    void removeVertex(size_t vertex);
    
    // Удалить ребро
    void removeEdge(size_t from, size_t to);
    
    // Получить степень вершины
    size_t degree(size_t vertex) const;
    
    // Проверить, является ли вершина листом
    bool isLeaf(size_t vertex) const;
    
    // Очистить граф
    void clear();

    // Дополнительные методы
    void merge(const Graph& other);
    void renumberVertices(const std::map<size_t, size_t>& mapping);
};