#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

using namespace std;

struct Tunnel {
    int orig, dest, len;
};

class TestCase {
    int n_graves, n_tunnels;
    int future_lea_grave; // index of future Lea's grave

    vector<int> objects;    // objects at each grave
    multimap<int, Tunnel> tunnels;

    int lea_grave; // index of Lea's grave

public:
    explicit TestCase(istream& in) {
        in >> n_graves >> n_tunnels >> future_lea_grave;

        objects.resize(n_graves + 1);
        for (int i = 1; i <= n_graves; ++i)
            in >> objects[i];

        for (int i = 0; i < n_tunnels; ++i) {
            Tunnel t;
            in >> t.orig >> t.dest >> t.len;
            if (t.orig < t.dest) swap(t.orig, t.dest);

            tunnels.insert(make_pair(t.orig, t));
        }

        lea_grave = n_graves;
    }

    /**
     * Executes Dijkstra's shortest-path algorithm from the given source node,
     *  and returns distances to all nodes in the graph.
     * @param tunnels edges of the graph
     * @param num_vertices total number of vertices
     * @param start_node start node from which to count distances
     * @return vector of distances for every node in the graph
     */
    static vector<int> dijkstra(const multimap<int, Tunnel>& tunnels, int num_vertices, int start_node) {
        vector<int> dist(num_vertices, numeric_limits<int>::max());
        vector<bool> visited(num_vertices, false);

        // NOTE this would be more efficient with a Fibonacci Heap
        typedef pair<int, int> pi;
        priority_queue<pi, vector<pi>, greater<pi>> priorityQueue; // priority queue of elements <distance, node>
        priorityQueue.push(make_pair(0, start_node)); // start_node is at distance 0
        dist[start_node] = 0;
        while (! priorityQueue.empty()) {
            auto p = priorityQueue.top(); priorityQueue.pop();
            int node = p.second;
            visited[node] = true;

            // Check edges to other cities
            for (auto it = tunnels.equal_range(node); it.first != it.second; ++it.first) {
                Tunnel t = it.first->second;
                int distToNeighbor = p.first + t.len;
                if (!visited[t.dest] and distToNeighbor < dist[t.dest]) {
                    dist[t.dest] = distToNeighbor;
                    priorityQueue.push(make_pair(distToNeighbor, t.dest));
                    // NOTE We should delete the previous pair containing this node
                }
            }
        }

        return dist;
    }

    // Only works for acyclic graphs
    static int value_of_node(const multimap<int, Tunnel>& edges, const vector<int>& objects, int target_node, vector<int>& cache) {
        if (cache[target_node] != -1) return cache[target_node];

        int best = 0;
        for (auto it = edges.equal_range(target_node); it.first != it.second; ++it.first) {
            Tunnel t = it.first->second;
            best = max(best, value_of_node(edges, objects, t.dest, cache));
        }

        cache[target_node] = objects[target_node] + best;
        return cache[target_node];
    }

    string solve() {
        // Get graves usable by Lea (from which she is closer than her future self)
        vector<int> lea_dist = dijkstra(tunnels, n_graves + 1, lea_grave);

        // Make tunnels undirected for future Lea
        multimap<int, Tunnel> undirected_tunnels;
        for (const auto& p : tunnels) {
            Tunnel t = p.second, r;
            undirected_tunnels.insert(make_pair(t.orig, t));

            r.len = t.len; r.orig = t.dest; r.dest = t.orig; // reverse Tunnel
            undirected_tunnels.insert(make_pair(r.orig, r));
        }
        vector<int> future_lea_dist = dijkstra(undirected_tunnels, n_graves + 1, future_lea_grave);

        if (future_lea_dist[0] <= lea_dist[0])
            return "impossible"; // impossible to get to the surface before future Lea

        for (int i = 1; i <= n_graves; ++i) {
            // If future Lea gets to this grave before Lea
            if (future_lea_dist[i] <= lea_dist[i]) {
                // Delete grave from graph
                for (auto it = tunnels.begin(); it != tunnels.end(); ) {
                    if (it->second.orig == i or it->second.dest == i)
                        it = tunnels.erase(it);
                    else
                        ++it;
                }
            }
        }

        // Execute another dijkstra with current tunnels to check if surface is still reachable
        lea_dist = dijkstra(tunnels, n_graves + 1, lea_grave);
        if (lea_dist[0] == numeric_limits<int>::max()) return "impossible";

        vector<int> cache(n_graves + 1, -1);

        multimap<int, Tunnel> reverse_tunnels;
        for (const auto& p : tunnels) {
            Tunnel r;
            r.len = p.second.len;
            r.orig = p.second.dest;
            r.dest = p.second.orig;
            reverse_tunnels.insert(make_pair(r.orig, r));
        }
        int ans = value_of_node(reverse_tunnels, objects, 0, cache) - value_of_node(reverse_tunnels, objects, lea_grave, cache);
        return to_string(ans);

        // Or use DP, in which the value of a node is its value + the maximum value of its connecting nodes
        // -> find value of surface node

        // Maximize objects carried to the surface
        // Set gain (opposite of cost) of an edge to the number of objects in its destination grave
        // Use Dijkstra to find the maximum cost path
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
