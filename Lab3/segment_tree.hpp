#pragma once
#include <vector>
#include <functional>

extern long long operations;

template<typename T>
class SegmentTree {
    std::vector<T> tree;
    int n;
    std::function<T(T, T)> op;
    T neutral;

    void build_tree(const std::vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            operations++;
        } else {
            int mid = (start + end) / 2;
            build_tree(arr, 2*node, start, mid);
            build_tree(arr, 2*node+1, mid+1, end);
            tree[node] = op(tree[2*node], tree[2*node+1]);
            operations++;
        }
    }

    T query_tree(int node, int start, int end, int l, int r) {
        operations++;
        if (r < start || end < l) return neutral;
        if (l <= start && end <= r) return tree[node];
        int mid = (start + end) / 2;
        return op(query_tree(2*node, start, mid, l, r), query_tree(2*node+1, mid+1, end, l, r));
    }

    void update_tree(int node, int start, int end, int pos, T value) {
        operations++;
        if (start == end) {
            tree[node] = value;
        } else {
            int mid = (start + end) / 2;
            if (pos <= mid) update_tree(2*node, start, mid, pos, value);
            else update_tree(2*node+1, mid+1, end, pos, value);
            tree[node] = op(tree[2*node], tree[2*node+1]);
        }
    }

public:
    SegmentTree(std::function<T(T, T)> operation, T neutral_element) 
        : op(operation), neutral(neutral_element) {}

    void build(const std::vector<T>& arr) {
        operations = 0;
        n = arr.size();
        tree.assign(4 * n, neutral);
        build_tree(arr, 1, 0, n-1);
    }

    T query(int l, int r) { return query_tree(1, 0, n-1, l, r); }
    void update(int pos, T value) { update_tree(1, 0, n-1, pos, value); }
    long long getBuildOps() const { return operations; }
};