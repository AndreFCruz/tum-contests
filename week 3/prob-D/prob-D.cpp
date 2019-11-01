// Week 3 - Problem D

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <queue>
#include <limits>

using namespace std;

typedef unsigned int uint;

class TestCase {
    uint num_vertices;
    multimap<uint, uint> edges;

public:
    explicit TestCase(istream& in) {
        in >> num_vertices;

        uint n1, n2;
        for (uint i = 0; i < num_vertices - 1; ++i) {
            in >> n1 >> n2;
            edges.insert(make_pair(n1, n2));
            edges.insert(make_pair(n2, n1));
        }
    }

    /**
     * Finds the maximum distance from anywhere to the given source node (through breadth-first search).
     * @param src the source node from which to count the distance.
     * @param parent a vector representing the parent of each node in this traversal.
     * @return a pair, representing the node which is farthest away from the src node, and its distance.
     *  <uint farthest_node, uint respective_distance>
     */
    pair<uint, uint> max_distance_bfs(uint src, vector<uint>& parent) {
        vector<bool> discovered(num_vertices, false);

        uint max_dist = 0;
        uint farthest_node = src;
        queue<pair<uint, uint>> q;  // queue of pairs <node_id, dist_to_source>
        q.push(make_pair(src, 0)); discovered[src] = true;
        while (! q.empty()) {
            uint node = q.front().first, dist = q.front().second;
            q.pop();
            if (dist > max_dist) {
                max_dist = dist;
                farthest_node = node;
            }

            for (auto it = edges.equal_range(node); it.first != it.second; ++it.first) {
                uint dest = it.first->second;
                if (! discovered[dest]) {
                    parent[dest] = node;
                    discovered[dest] = true;
                    q.push(make_pair(dest, dist + 1));
                }
            }
        }

        return make_pair(farthest_node, max_dist);
    }

    uint solve() {
        uint src = 0;   // arbitrary source node
        vector<uint> parent(num_vertices);
        auto t = max_distance_bfs(src, parent);
        auto u = max_distance_bfs(t.first, parent);

        // path <u, t> is the tree's diameter
        uint diameter_length = u.second;

        // Rebuild path from t to u, the center of the tree is the middle vertex
        uint count = 0;
        uint center = u.first;
        while (count++ < diameter_length / 2)
            center = parent[center];

        return center;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}