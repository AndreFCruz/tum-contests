// Week 4 - Problem E

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <limits>

using namespace std;

struct Edge {
    uint origin, dest, cost;

    Edge reverse() const {
        Edge reverse;
        reverse.origin = this->dest;
        reverse.dest = this->origin;
        reverse.cost = this->cost;
        return reverse;
    }
};

class TestCase {
    uint n_junctions;
    multimap<uint, Edge> edges;
    vector<uint> daily_shortest_path;

public:
    explicit TestCase(istream& in) {
        uint n_paths;   // number of streets and footpaths
        uint n_junctions_daily_path; // number of vertices in the shortest path

        in >> n_junctions >> n_paths >> n_junctions_daily_path;

        daily_shortest_path = vector<uint>(n_junctions_daily_path);
        for (uint i = 0; i < n_junctions_daily_path; ++i) {
            in >> daily_shortest_path[i];
        }

        for (uint i = 0; i < n_paths; ++i) {
            Edge e;
            in >> e.origin >> e.dest >> e.cost;
            edges.insert(make_pair(e.origin, e));
            edges.insert(make_pair(e.dest, e.reverse()));
        }
    }

    /**
     * Executes Dijkstra's shortest-path algorithm from the given source node,
     *  and returns distances to all nodes in the graph.
     * @param edges edges of the graph
     * @param num_vertices total number of vertices
     * @param start_node start node from which to count distances
     * @return matrix of possible predecessors for each vertex
     */
    static vector<vector<uint>> dijkstra(const multimap<uint, Edge>& edges, uint num_vertices, uint start_node) {
        vector<uint> dist(num_vertices, numeric_limits<uint>::max());
        vector<bool> visited(num_vertices, false);
        vector<vector<uint>> predecessor(num_vertices, vector<uint>());

        // NOTE this would be more efficient with a Fibonacci Heap
        typedef pair<uint, uint> pi;
        priority_queue<pi, vector<pi>, greater<pi>> priorityQueue; // priority queue of elements <distance, node>
        priorityQueue.push(make_pair(0, start_node)); // start_node is at distance 0
        dist[start_node] = 0;
        while (! priorityQueue.empty()) {
            auto p = priorityQueue.top(); priorityQueue.pop();
            uint node = p.second;
            visited[node] = true;

            // Check edges to other cities
            for (auto it = edges.equal_range(node); it.first != it.second; ++it.first) {
                Edge e = it.first->second;
                uint distToNeighbor = p.first + e.cost;
                if (!visited[e.dest] and distToNeighbor <= dist[e.dest]) {
                    if (distToNeighbor == dist[e.dest]) {
                        predecessor[e.dest].push_back(e.origin);
                    } else {
                        predecessor[e.dest] = vector<uint>{e.origin};
                        dist[e.dest] = distToNeighbor;
                        priorityQueue.push(make_pair(distToNeighbor, e.dest));
                        // NOTE We should delete the previous pair containing this node
                    }
                }
            }
        }

        return predecessor;
    }

    string solve() {
//        cout << "EDGES:\n";
//        for (const auto& p : edges) // TODO delete
//            cout << p.second.origin << " -> " << p.second.dest << " with w=" << p.second.cost << endl;

        // Check if any edge in the path can be changed
        for (uint i = 0; i < daily_shortest_path.size() - 1; ++i) {
            uint node = daily_shortest_path[i], next_node = daily_shortest_path[i+1];

            uint min_edge = numeric_limits<uint>::max();
            for (auto it = edges.equal_range(node); it.first != it.second; ++it.first) {
                Edge e = it.first->second;
                if (e.dest == next_node) {
                    if (e.cost < min_edge) min_edge = e.cost;
                    else if (e.cost == min_edge) return "yes";
                }
            }
        }

        // Check if any node in the path can be changed
        auto predecessors = dijkstra(edges, this->n_junctions + 1, 1);
        for (uint node : daily_shortest_path) {
            if (predecessors[node].size() > 1) return "yes";
        }

        return "no";
    }
};

int main() {
    ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
