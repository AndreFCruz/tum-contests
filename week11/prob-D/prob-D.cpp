#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

/**
 * Lowest Common Ancestor
 * From: https://cp-algorithms.com/graph/lca.html
 */
struct LCA {
    vector<int> height, euler, first, segtree;
    vector<bool> visited;
    int n;

    LCA(const multimap<int, int>& adj, int root = 0) {
        n = adj.size();
        height.resize(n);
        first.resize(n);
        euler.reserve(n * 2);
        visited.assign(n, false);
        dfs(adj, root);
        int m = euler.size();
        segtree.resize(m * 4);
        build(1, 0, m - 1);
    }

    void dfs(const multimap<int, int>& adj, int node, int h = 0) {
        visited[node] = true;
        height[node] = h;
        first[node] = euler.size();
        euler.push_back(node);
        for (auto it = adj.equal_range(node); it.first != it.second; ++it.first) {
            assert(it.first->first == node);
            int to = it.first->second;
            if (!visited[to]) {
                dfs(adj, to, h + 1);
                euler.push_back(node);
            }
        }
    }

    void build(int node, int b, int e) {
        if (b == e) {
            segtree[node] = euler[b];
        } else {
            int mid = (b + e) / 2;
            build(node << 1, b, mid);
            build(node << 1 | 1, mid + 1, e);
            int l = segtree[node << 1], r = segtree[node << 1 | 1];
            segtree[node] = (height[l] < height[r]) ? l : r;
        }
    }

    int query(int node, int b, int e, int L, int R) {
        if (b > R || e < L)
            return -1;
        if (b >= L && e <= R)
            return segtree[node];
        int mid = (b + e) >> 1;

        int left = query(node << 1, b, mid, L, R);
        int right = query(node << 1 | 1, mid + 1, e, L, R);
        if (left == -1) return right;
        if (right == -1) return left;
        return height[left] < height[right] ? left : right;
    }

    int lca(int u, int v) {
        int left = first[u], right = first[v];
        if (left > right)
            swap(left, right);
        return query(1, 0, euler.size() - 1, left, right);
    }
};


class TestCase {
    int n_nodes;
    multimap<int, int> edges; // use unordered_multimap ?
    vector<int> target_nodes;

public:
    explicit TestCase() {
        cin >> n_nodes;

        int c, b;
        for (int i = 0; i < n_nodes; ++i) {
            cin >> c;
            for (int j = 0; j < c; ++j) {
                cin >> b;
                edges.insert(make_pair(i, b - 1));
                edges.insert(make_pair(b - 1, i));
            }
        }

        int v;
        cin >> v;
        target_nodes.resize(v);
        for (int i = 0; i < v; ++i) {
            cin >> target_nodes[i];
            target_nodes[i] -= 1;
        }
    }

    int solve() {
        if (n_nodes == 1 or edges.size() == 0) return 0;
        LCA euler_lca(edges, 0);

        int branches = 0;
        int curr_node = 0;
        // LCA of each two consecutive nodes
        for (int target : target_nodes) {
            int lca = euler_lca.lca(curr_node, target);
            int dist_curr_lca = euler_lca.height[curr_node]- euler_lca.height[lca];
            int dist_lca_target = euler_lca.height[target] - euler_lca.height[lca];
            assert(dist_curr_lca >= 0 and dist_lca_target >= 0);

            branches += dist_curr_lca + dist_lca_target;
            curr_node = target;
        }

        return branches;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase().solve() << endl;
    }

    return EXIT_SUCCESS;
}
