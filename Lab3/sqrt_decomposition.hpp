#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>

extern long long operations;

template<typename T>
class SqrtDecomposition {
    std::vector<T> blocks;
    std::vector<T> arr;
    int block_size;
    int num_blocks;
    std::function<T(T, T)> op;
    T neutral;
public:
    SqrtDecomposition(std::function<T(T, T)> operation, T neutral_element) 
        : op(operation), neutral(neutral_element) {}

    void build(const std::vector<T>& a) {
        operations = 0;
        arr = a;
        int n = arr.size();
        block_size = std::max(1, (int)std::sqrt(n));
        num_blocks = (n + block_size - 1) / block_size;
        blocks.assign(num_blocks, neutral);
        for (int i = 0; i < n; i++) {
            int block_idx = i / block_size;
            blocks[block_idx] = op(blocks[block_idx], arr[i]);
            operations++;
        }
    }

    T query(int l, int r) {
        T result = neutral;
        int start_block = l / block_size;
        int end_block = r / block_size;

        if (start_block == end_block) {
            for (int i = l; i <= r; i++) { result = op(result, arr[i]); operations++; }
        } else {
            for (int i = l; i < (start_block + 1) * block_size; i++) { result = op(result, arr[i]); operations++; }
            for (int b = start_block + 1; b < end_block; b++) { result = op(result, blocks[b]); operations++; }
            for (int i = end_block * block_size; i <= r; i++) { result = op(result, arr[i]); operations++; }
        }
        return result;
    }

    long long getBuildOps() const { return operations; }
};