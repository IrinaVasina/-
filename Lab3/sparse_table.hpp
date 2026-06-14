#pragma once
#include <vector>
#include <algorithm>
#include <cmath>

extern long long operations;

class SparseTable {
    std::vector<std::vector<int>> st;
    std::vector<int> log_table;
    int n;
public:
    void build(const std::vector<int>& arr) {
        operations = 0;
        n = arr.size();
        log_table.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) {
            log_table[i] = log_table[i/2] + 1;
            operations++;
        }
        int k = log_table[n] + 1;
        st.assign(n, std::vector<int>(k));
        for (int i = 0; i < n; i++) {
            st[i][0] = arr[i];
            operations++;
        }
        for (int j = 1; j < k; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[i][j] = std::min(st[i][j-1], st[i + (1 << (j-1))][j-1]);
                operations++;
            }
        }
    }

    int query(int l, int r) {
        operations++;
        int len = r - l + 1;
        int k = log_table[len];
        return std::min(st[l][k], st[r - (1 << k) + 1][k]);
    }

    long long getBuildOps() const { return operations; }
};