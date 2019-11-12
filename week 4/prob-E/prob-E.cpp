// Week 4 - Problem E

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <limits>

using namespace std;

struct Edge {
    uint origin, dest, cost;
    bool used = false;

    explicit Edge() = default;
    Edge(uint o, uint d, uint c, bool u) : origin(o), dest(d), cost(c), used(u) {}

    Edge reverse() const {
        Edge reverse;
        reverse.origin = this->dest;
        reverse.dest = this->origin;
        reverse.cost = this->cost;
        return reverse;
    }
};

typedef vector<vector<Edge>> Graph;

class TestCase {
    uint n_junctions;
    vector<uint> daily_shortest_path;
    Graph graph;

public:
    explicit TestCase(istream& in) {
        uint n_paths;   // number of streets and footpaths
        uint n_junctions_daily_path; // number of vertices in the shortest path

        in >> n_junctions >> n_paths >> n_junctions_daily_path;

        daily_shortest_path = vector<uint>(n_junctions_daily_path);
        for (uint i = 0; i < n_junctions_daily_path; ++i) {
            in >> daily_shortest_path[i];
        }

        graph = vector<vector<Edge>>(n_junctions + 1, vector<Edge>());
        for (uint i = 0; i < n_paths; ++i) {
            Edge e;
            in >> e.origin >> e.dest >> e.cost;
            graph[e.origin].push_back(e);
            graph[e.dest].push_back(e.reverse());
        }
    }

    /**
     * Executes Dijkstra's shortest-path algorithm from the given source node,
     *  and returns distances to all nodes in the graph.
     * @param graph a graph (a collection of edges)
     * @param distances a vector of distances from the source node, already initialized
     * @param predecessors a vector of vectors of predecessors, already initialized
     * @param num_vertices number of vertices in the graph
     * @param start_node the start node for Dijkstra's algorithm
     */
    static void dijkstra(
            Graph& graph,
            vector<uint>& distances,
            vector<vector<uint>>& predecessors,
            uint num_vertices, uint start_node)
    {
        vector<bool> nodes_visited = vector<bool>(num_vertices + 1, false);
        predecessors[start_node].push_back(0);
        nodes_visited[start_node] = true;
        distances[start_node] = 0;

        // Priority queue orders edges by increasing weight
        auto compare_edges = [](const Edge& l, const Edge& r) { return (l.cost > r.cost); };
        priority_queue<Edge, vector<Edge>, decltype(compare_edges)> pq(compare_edges);

        // push the connections from the start
        for (const auto &e : graph[start_node])
            pq.push(e);

        while (!pq.empty()) {
            // take the closest node
            Edge top_e = pq.top(); pq.pop();

            if (! nodes_visited[top_e.dest]) {
                nodes_visited[top_e.dest] = true;
                distances[top_e.dest] = top_e.cost;
                predecessors[top_e.dest] = vector<u_int>({top_e.origin});
                for (auto &edg: graph[top_e.dest]) {
                    if (! edg.used) {
                        pq.push(Edge(edg.origin, edg.dest, distances[top_e.dest] + edg.cost, false));
                    }
                }
            } else if (top_e.cost == distances[top_e.dest]) {
                // if weight to get here is the same
                predecessors[top_e.dest].push_back(top_e.origin);
                for (auto &edg: graph[top_e.dest]) {
                    if (!edg.used) {
                        edg.used = true;
                        pq.push(Edge(edg.origin, edg.dest, distances[top_e.dest] + edg.cost, true));
                    }
                }
            }
        }
    }

    string solve() {
        // Check if any node in the path can be changed
        vector<uint> distances(this->n_junctions + 1, numeric_limits<uint>::max());
        vector<vector<uint>> predecessors(this->n_junctions + 1, vector<uint>());
        dijkstra(this->graph, distances, predecessors, this->n_junctions + 1, 1);
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
