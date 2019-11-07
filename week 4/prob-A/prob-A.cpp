// Week 4 - Problem A

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

using namespace std;

struct Edge {
    uint origin, dest, cost;

    Edge reverse() {
        Edge reverse;
        reverse.origin = this->dest;
        reverse.dest = this->origin;
        reverse.cost = this->cost;
        return reverse;
    }
};

class TestCase {
    uint n_vertices;
    multimap<uint, Edge> edges;

public:
    TestCase(istream& in) {
        uint n_edges;
        in >> n_vertices >> n_edges;
        for (uint i = 0; i < n_edges; ++i) {
            Edge e;
            in >> e.origin >> e.dest >> e.cost;
            edges.insert(make_pair(e.origin, e));
            edges.insert(make_pair(e.dest, e.reverse()));
        }
    }

    uint solve(uint start_node = 1) {
        // Distances from source node
        vector<uint> dist(n_vertices + 1, numeric_limits<uint>::max());
        vector<bool> visited(n_vertices + 1, false);

        // NOTE this would be more efficient with a Fibonacci Heap
        typedef pair<uint, uint> pi;
        priority_queue<pi, vector<pi>, greater<pi>> priorityQueue; // priority queue of elements <distance, node>
        priorityQueue.push(make_pair(0, start_node)); // starts at node 1 with distance 0
        dist[start_node] = 0;
        while (! priorityQueue.empty()) {
            auto p = priorityQueue.top(); priorityQueue.pop();
            uint node = p.second;
            visited[node] = true;

            for (auto it = edges.equal_range(node); it.first != it.second; ++it.first) {
                Edge e = it.first->second;
                uint distToNeighbor = p.first + e.cost;
                if (!visited[e.dest] and distToNeighbor < dist[e.dest]) {
                    dist[e.dest] = distToNeighbor;
                    priorityQueue.push(make_pair(distToNeighbor, e.dest));
                    // NOTE We should delete the previous pair containing this node
                }
            }
        }

        return dist.back();
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