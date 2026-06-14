#pragma once
#include <vector>

extern long long operations;

template<typename T>
class FenwickTree {
    std::vector<T> bit;
    std::vector<T> arr;
    int n;
public:
    void build(const std::vector<T>& a) {
        operations = 0;
        n = a.size();
        arr = a;
        bit.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            int idx = i + 1;
            while (idx <= n) {
                bit[idx] += arr[i];
                operations++;
                idx += idx & (-idx);
            }
        }
    }

    T query(int l, int r) {
        T res = 0;
        int idx = r + 1;
        while (idx > 0) { res += bit[idx]; operations++; idx -= idx & (-idx); }
        idx = l;
        while (idx > 0) { res -= bit[idx]; operations++; idx -= idx & (-idx); }
        return res;
    }

    void update(int pos, T value) {
        int delta = value - arr[pos];
        arr[pos] = value;
        int idx = pos + 1;
        while (idx <= n) {
            bit[idx] += delta;
            operations++;
            idx += idx & (-idx);
        }
    }

    long long getBuildOps() const { return operations; }
};