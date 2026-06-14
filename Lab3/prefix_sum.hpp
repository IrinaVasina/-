#pragma once
#include <vector>

extern long long operations;

class PrefixSum1D {
    std::vector<long long> pref;
    int n;
public:
    void build(const std::vector<int>& arr) {
        operations = 0;
        n = arr.size();
        pref.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            pref[i + 1] = pref[i] + arr[i];
            operations++; 
        }
    }
    long long query(int l, int r) {
        operations++; 
        return pref[r + 1] - pref[l];
    }
    long long getBuildOps() const { return operations; }
};

class PrefixSum2D {
    std::vector<std::vector<long long>> pref;
    int n, m;
public:
    void build(const std::vector<std::vector<int>>& arr) {
        operations = 0;
        n = arr.size();
        m = arr[0].size();
        pref.assign(n + 1, std::vector<long long>(m + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                pref[i][j] = pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1] + arr[i-1][j-1];
                operations++; 
            }
        }
    }
    long long query(int x1, int y1, int x2, int y2) {
        operations++;
        return pref[x2+1][y2+1] - pref[x1][y2+1] - pref[x2+1][y1] + pref[x1][y1];
    }
    long long getBuildOps() const { return operations; }
};