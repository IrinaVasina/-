#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <functional>

// Подключаем наши структуры
#include "prefix_sum.hpp"
#include "rmq_naive.hpp"
#include "sqrt_decomposition.hpp"
#include "segment_tree.hpp"
#include "fenwick_tree.hpp"
#include "sparse_table.hpp"

// Глобальный счетчик (определение)
long long operations = 0;

struct Measurement {
    int n;
    long long build_ops;
    double avg_query_ops;
};

// Генерация данных
std::vector<int> genArray(int n) {
    std::vector<int> a(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    for(int &x : a) x = dis(gen);
    return a;
}

// Генерация 2D матрицы
std::vector<std::vector<int>> genMatrix(int n, int m) {
    std::vector<std::vector<int>> mat(n, std::vector<int>(m));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            mat[i][j] = dis(gen);
    return mat;
}

std::vector<std::pair<int,int>> genQueries(int n, int count) {
    std::vector<std::pair<int,int>> q;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, n-1);
    for(int i=0; i<count; ++i) {
        int l = dis(gen), r = dis(gen);
        if(l > r) std::swap(l, r);
        q.push_back({l, r});
    }
    return q;
}

void saveCSV(const std::string& name, const std::vector<Measurement>& data) {
    std::ofstream f("measurements.csv", std::ios::app);
    f << name << "\nn,build_ops,avg_query_ops\n";
    for(auto &m : data)
        f << m.n << "," << m.build_ops << "," << std::fixed << std::setprecision(2) << m.avg_query_ops << "\n";
    f << "\n";
}

int main() {
    std::ofstream clear("measurements.csv", std::ios::trunc); 
    std::vector<Measurement> res;

    std::cout << "=== Starting measurements ===" << std::endl;

    // 1. Prefix Sum 1D
    std::cout << "1. Testing Prefix Sum 1D..." << std::endl;
    {
        PrefixSum1D ps;
        std::vector<int> sizes = {10, 50, 100, 500, 1000, 5000};
        for(int n : sizes) {
            auto arr = genArray(n);
            auto qs = genQueries(n, 1000);
            ps.build(arr);
            long long b_ops = ps.getBuildOps();
            long long q_ops = 0;
            for(auto &q : qs) {
                operations = 0;
                ps.query(q.first, q.second);
                q_ops += operations;
            }
            res.push_back({n, b_ops, (double)q_ops/qs.size()});
        }
        saveCSV("PrefixSum1D", res); res.clear();
    }

    // 2. Prefix Sum 2D (НОВОЕ!)
    std::cout << "2. Testing Prefix Sum 2D..." << std::endl;
    {
        PrefixSum2D ps2d;
        std::vector<int> sizes = {10, 20, 50, 100, 200};
        for(int n : sizes) {
            auto mat = genMatrix(n, n);
            ps2d.build(mat);
            long long b_ops = ps2d.getBuildOps();
            
            // Тестовые запросы
            long long q_ops = 0;
            int num_queries = 1000;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, n-1);
            
            for(int i = 0; i < num_queries; i++) {
                operations = 0;
                int x1 = dis(gen), y1 = dis(gen);
                int x2 = dis(gen), y2 = dis(gen);
                if(x1 > x2) std::swap(x1, x2);
                if(y1 > y2) std::swap(y1, y2);
                ps2d.query(x1, y1, x2, y2);
                q_ops += operations;
            }
            res.push_back({n, b_ops, (double)q_ops/num_queries});
        }
        saveCSV("PrefixSum2D", res); res.clear();
    }

    // 3. RMQ Naive (O(n^2) - маленькие размеры!)
    std::cout << "3. Testing RMQ Naive..." << std::endl;
    {
        RMQAllSegments rmq;
        std::vector<int> sizes = {10, 20, 50, 100, 200}; 
        for(int n : sizes) {
            auto arr = genArray(n);
            auto qs = genQueries(n, 1000);
            rmq.build(arr);
            long long b_ops = rmq.getBuildOps();
            long long q_ops = 0;
            for(auto &q : qs) {
                operations = 0;
                rmq.query(q.first, q.second);
                q_ops += operations;
            }
            res.push_back({n, b_ops, (double)q_ops/qs.size()});
        }
        saveCSV("RMQ_Naive", res); res.clear();
    }

    // 4. Sqrt Decomposition
    std::cout << "4. Testing Sqrt Decomposition..." << std::endl;
    {
        SqrtDecomposition<int> sq([](int a, int b){return a+b;}, 0);
        std::vector<int> sizes = {10, 100, 500, 1000, 5000, 10000};
        for(int n : sizes) {
            auto arr = genArray(n);
            auto qs = genQueries(n, 1000);
            sq.build(arr);
            long long b_ops = sq.getBuildOps();
            long long q_ops = 0;
            for(auto &q : qs) {
                operations = 0;
                sq.query(q.first, q.second);
                q_ops += operations;
            }
            res.push_back({n, b_ops, (double)q_ops/qs.size()});
        }
        saveCSV("SqrtDecomposition", res); res.clear();
    }

    // 5. Segment Tree
    std::cout << "5. Testing Segment Tree..." << std::endl;
    {
        SegmentTree<int> st([](int a, int b){return std::min(a,b);}, 1e9);
        std::vector<int> sizes = {10, 100, 500, 1000, 5000, 10000};
        for(int n : sizes) {
            auto arr = genArray(n);
            auto qs = genQueries(n, 1000);
            st.build(arr);
            long long b_ops = st.getBuildOps();
            long long q_ops = 0;
            for(auto &q : qs) {
                operations = 0;
                st.query(q.first, q.second);
                q_ops += operations;
            }
            res.push_back({n, b_ops, (double)q_ops/qs.size()});
        }
        saveCSV("SegmentTree", res); res.clear();
    }

    // 6. Segment Tree Update (НОВОЕ!)
    std::cout << "6. Testing Segment Tree Update..." << std::endl;
    {
        SegmentTree<int> st([](int a, int b){return a+b;}, 0);
        std::vector<int> sizes = {100, 500, 1000, 5000, 10000};
        for(int n : sizes) {
            auto arr = genArray(n);
            st.build(arr);
            long long b_ops = st.getBuildOps();
            
            // Замер обновления
            operations = 0;
            int num_updates = 1000;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis_pos(0, n-1);
            std::uniform_int_distribution<> dis_val(1, 1000);
            
            for(int i = 0; i < num_updates; i++) {
                int pos = dis_pos(gen);
                int val = dis_val(gen);
                st.update(pos, val);
            }
            double avg_update_ops = (double)operations / num_updates;
            
            res.push_back({n, b_ops, avg_update_ops});
        }
        saveCSV("SegmentTree_Update", res); res.clear();
    }

    // 7. Fenwick Tree
    std::cout << "7. Testing Fenwick Tree..." << std::endl;
    {
        FenwickTree<int> ft;
        std::vector<int> sizes = {10, 100, 500, 1000, 5000, 10000};
        for(int n : sizes) {
            auto arr = genArray(n);
            auto qs = genQueries(n, 1000);
            ft.build(arr);
            long long b_ops = ft.getBuildOps();
            long long q_ops = 0;
            for(auto &q : qs) {
                operations = 0;
                ft.query(q.first, q.second);
                q_ops += operations;
            }
            res.push_back({n, b_ops, (double)q_ops/qs.size()});
        }
        saveCSV("FenwickTree", res); res.clear();
    }

    // 8. Fenwick Tree Update (НОВОЕ!)
    std::cout << "8. Testing Fenwick Tree Update..." << std::endl;
    {
        FenwickTree<int> ft;
        std::vector<int> sizes = {100, 500, 1000, 5000, 10000};
        for(int n : sizes) {
            auto arr = genArray(n);
            ft.build(arr);
            long long b_ops = ft.getBuildOps();
            
            // Замер обновления
            operations = 0;
            int num_updates = 1000;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis_pos(0, n-1);
            std::uniform_int_distribution<> dis_val(1, 1000);
            
            for(int i = 0; i < num_updates; i++) {
                int pos = dis_pos(gen);
                int val = dis_val(gen);
                ft.update(pos, val);
            }
            double avg_update_ops = (double)operations / num_updates;
            
            res.push_back({n, b_ops, avg_update_ops});
        }
        saveCSV("FenwickTree_Update", res); res.clear();
    }

    // 9. Sparse Table
    std::cout << "9. Testing Sparse Table..." << std::endl;
    {
        SparseTable st;
        std::vector<int> sizes = {10, 100, 500, 1000, 5000, 10000};
        for(int n : sizes) {
            auto arr = genArray(n);
            auto qs = genQueries(n, 1000);
            st.build(arr);
            long long b_ops = st.getBuildOps();
            long long q_ops = 0;
            for(auto &q : qs) {
                operations = 0;
                st.query(q.first, q.second);
                q_ops += operations;
            }
            res.push_back({n, b_ops, (double)q_ops/qs.size()});
        }
        saveCSV("SparseTable", res); res.clear();
    }

    std::cout << "\n=== All measurements completed! ===" << std::endl;
    std::cout << "Check measurements.csv" << std::endl;
    return 0;
}