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
     * Finds the maximum distance from anywhere to the given source node.
     * @param src the source node from which to count the distance.
     * @return the maximum distance betweent the src node and any other node.
     */
    uint max_distance(uint src) {
        vector<bool> discovered(num_vertices, false);

        uint max_dist = 0;
        queue<pair<uint, uint>> q;  // queue of pairs <node_id, dist_to_source>
        q.push(make_pair(src, 0)); discovered[src] = true;
        while (! q.empty()) {
            uint node = q.front().first, dist = q.front().second;
            q.pop();
            if (dist > max_dist) max_dist = dist;

            for (auto it = edges.equal_range(node); it.first != it.second; ++it.first) {
                uint dest = it.first->second;
                if (! discovered[dest]) {
                    discovered[dest] = true;
                    q.push(make_pair(dest, dist + 1));
                }
            }
        }

        return max_dist;
    }

    uint solve() {
        uint min_max_distance = numeric_limits<uint>::max();
        uint best_node = numeric_limits<uint>::max();

        for (uint i = 0; i < num_vertices; ++i) {
            uint max_dist = max_distance(i);
            if (min_max_distance > max_dist) {
                min_max_distance = max_dist;
                best_node = i;
            }
        }

        return best_node;
    }
};

int main() {
    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}