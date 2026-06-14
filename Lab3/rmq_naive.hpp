#pragma once
#include <vector>
#include <algorithm>

extern long long operations;

class RMQAllSegments {
    std::vector<std::vector<int>> table;
    int n;
public:
    void build(const std::vector<int>& arr) {
        operations = 0;
        n = arr.size();
        table.assign(n, std::vector<int>(n));
        for (int i = 0; i < n; i++) {
            table[i][i] = arr[i];
            operations++;
        }
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                table[i][j] = std::min(table[i][j-1], arr[j]);
                operations++; 
            }
        }
    }
    int query(int l, int r) {
        operations++;
        return table[l][r];
    }
    long long getBuildOps() const { return operations; }
};