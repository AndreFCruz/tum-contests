// Week 4 - Problem C

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <limits>
#include <cmath>

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
    uint n_cities;  // n
    uint n_roads;   // m
    uint n_markets; // s
    uint start_node;    // a
    uint end_node;      // b

    // Roads between cities
    multimap<uint, Edge> edges;

    // Mapping from market's true node to fictional node (and reverse)
//    unordered_map<uint, uint> true_to_fictional;
//    unordered_map<uint, uint> fictional_to_true;

    // Supermarkets per city, there may be multiple per city but we only care about the fastest
    unordered_map<uint, uint> markets;   // markets[market_node] = market_time

public:
    explicit TestCase(istream& in) {
        in >> n_cities >> n_roads >> n_markets >> start_node >> end_node;

        for (uint i = 0; i < n_roads; ++i) {
            Edge e;
            in >> e.origin >> e.dest >> e.cost;

            // Two-way roads
            edges.insert(make_pair(e.origin, e));
            edges.insert(make_pair(e.dest, e.reverse()));
        }

        uint c, w;
        for (uint i = 0; i < n_markets; ++i) {
            in >> c >> w;

            // If there is no market at c, or if market at c is slower
            if (markets.count(c) == 0 or markets[c] > w)
                markets[c] = w;
        }
    }

    static string minutes_to_time_format(uint mins) {
        uint hours = mins / 60;
        mins -= hours * 60;

        char formatted[6];
        uint n = sprintf(formatted, "%01u:%02u", hours, mins);
        formatted[n] = 0;
        return string(formatted);
    }

    /**
     * Executes Dijkstra's shortest-path algorithm from the given source node,
     *  and returns distances to all nodes in the graph.
     * @param edges edges of the graph
     * @param num_vertices total number of vertices
     * @param start_node start node from which to count distances
     * @return vector of distances for every node in the graph
     */
    static vector<uint> dijkstra(const multimap<uint, Edge>& edges, uint num_vertices, uint start_node) {
        vector<uint> dist(num_vertices, numeric_limits<uint>::max());
        vector<bool> visited(num_vertices, false);

        // NOTE this would be more efficient with a Fibonacci Heap
        typedef pair<uint, uint> pi;
        priority_queue<pi, vector<pi>, greater<pi>> priorityQueue; // priority queue of elements <distance, node>
        priorityQueue.push(make_pair(0, start_node)); // starts at node 1 with distance 0
        dist[start_node] = 0;
        while (! priorityQueue.empty()) {
            auto p = priorityQueue.top(); priorityQueue.pop();
            uint node = p.second;
            visited[node] = true;

            // Check edges to other cities
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

        return dist;
    }

    string solve() {
        // Do Dijkstra shortest-path
        // maintain 2 distance vectors: one as normal, and another for paths which have gone through a supermarket
        // on every vertex that has a supermarket, visit it
        // when comparing distances:
        // if node has supermarket: market_dist[n] = min(market_dist[n], regular_dist[n] + market.time)
        // else: market_dist[n] = market_dist[parent] != inf ? market_dist[parent] + edge.cost : inf
        // regular_dist is updates as normal

        // OR:
        // "Find the shortest paths from start and destination then add both for every
        //  supermarket and find the minimum. This way you only use dijkstra twice not
        //  number of supermarket many times"
        // (brute-force but there are at most 10000 supermarkets)

        // New set of edges, containing fictional edges to/from super-markets
//        for (const auto& p : markets) {
//            n_fictional_nodes += 1;
//
//            Edge e;
//            e.origin = p.first;             // true market node
//            e.dest = n_fictional_nodes;     // fictional market node
//            e.cost = p.second;              // cost to visit market
//            extended_edges.insert(make_pair(e.origin, e));
//
//            Edge reverse = e.reverse();     // Edge from market node to city node, with cost 0
//            reverse.cost = 0;
//            extended_edges.insert(make_pair(e.dest, reverse));
//        }

        // Reverse edges for dijkstra from destination node
        multimap<uint, Edge> reverse_edges;
        for (const auto& p : edges)
            reverse_edges.insert(make_pair(p.second.dest, p.second.reverse()));

        // Distances from source node
        vector<uint> dist_from_src = TestCase::dijkstra(edges, n_cities + 1, start_node);

        // Distances from destination node ** with all edges reversed **
        vector<uint> dist_from_dest = TestCase::dijkstra(reverse_edges, n_cities + 1, end_node);

        // Sum distances to and from all super-markets' fictional nodes
        uint min_dist = numeric_limits<uint>::max();
        for (const auto& market : this->markets) {
            uint node = market.first, queue_time = market.second;
            if (dist_from_src[node] != numeric_limits<uint>::max() and dist_from_dest[node] != numeric_limits<uint>::max())
                min_dist = min(min_dist, dist_from_src[node] + dist_from_dest[node] + queue_time);
        }

        return min_dist == numeric_limits<uint>::max() ? "impossible" : minutes_to_time_format(min_dist);
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
