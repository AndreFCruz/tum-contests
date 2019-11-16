#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

using namespace std;

class MaxFlowAlgorithm {
public:
    virtual void AddEdge (int from, int to, int cap) = 0;
    virtual int GetMaxFlow(int source, int sink) = 0;
    virtual ~MaxFlowAlgorithm() = default;
};

// Push-Relabel Max-Flow Algorithm from https://codeforces.com/blog/entry/14378
struct Edge {
    int from, to, index;
    int cap, flow;

    Edge(int from, int to, int cap, int flow, int index): from(from), to(to), cap(cap), flow(flow), index(index) {}
};

class PushRelabel : public MaxFlowAlgorithm {
    int n;
    vector <vector <Edge>> adj;
    vector <int> excess;
    vector <int> dist, count;
    vector <bool> active;
    vector <vector <int>> B;
    int b;
    queue <int> Q;

public:
    explicit PushRelabel (int n): n(n), adj(n) {}

    ~PushRelabel() = default;

    void AddEdge (int from, int to, int cap) {
        adj[from].push_back(Edge (from, to, cap, 0, adj[to].size()));
        if (from == to) {
            adj[from].back().index++;
        }
        adj[to].push_back(Edge (to, from, 0, 0, adj[from].size() - 1));

    }

    void Enqueue (int v) {
        if (!active[v] && excess[v] > 0 && dist[v] < n) {
            active[v] = true;
            B[dist[v]].push_back(v);
            b = max(b, dist[v]);
        }
    }

    void Push (Edge  &e) {
        int amt = min(excess[e.from], e.cap - e.flow);
        if (dist[e.from] == dist[e.to] + 1 && amt > int(0)) {
            e.flow += amt;
            adj[e.to][e.index].flow -= amt;
            excess[e.to] += amt;
            excess[e.from] -= amt;
            Enqueue(e.to);
        }
    }

    void Gap (int k) {
        for (int v = 0; v < n; v++) if (dist[v] >= k) {
                count[dist[v]]--;
                dist[v] = max(dist[v], n);
                count[dist[v]]++;
                Enqueue(v);
            }
    }

    void Relabel (int v) {
        count[dist[v]]--;
        dist[v] = n;
        for (auto e: adj[v]) if (e.cap - e.flow > 0) {
                dist[v] = min(dist[v], dist[e.to] + 1);
            }
        count[dist[v]]++;
        Enqueue(v);
    }

    void Discharge(int v) {
        for (auto &e: adj[v]) {
            if (excess[v] > 0) {
                Push(e);
            } else {
                break;
            }
        }

        if (excess[v] > 0) {
            if (count[dist[v]] == 1) {
                Gap(dist[v]);
            } else {
                Relabel(v);
            }
        }
    }

    int GetMaxFlow (int s, int t) {
        dist = vector <int>(n, 0), excess = vector<int>(n, 0), count = vector <int>(n + 1, 0), active = vector <bool>(n, false), B = vector <vector <int>>(n), b = 0;

        for (auto &e: adj[s]) {
            excess[s] += e.cap;
        }

        count[0] = n;
        Enqueue(s);
        active[t] = true;

        while (b >= 0) {
            if (!B[b].empty()) {
                int v = B[b].back();
                B[b].pop_back();
                active[v] = false;
                Discharge(v);
            } else {
                b--;
            }
        }
        return excess[t];
    }
};

class EdmondsKarp : public MaxFlowAlgorithm {
    class Edge {
        Edge(int src, int dst, int cap) : src(src), dest(dst), cap(cap) {}

    public:
        int src, dest, cap;    // source, destination, capacity
        int flow = 0;           // flow (may be negative if in opposite direction)
        Edge * reverse = nullptr;

        static pair<Edge*, Edge*> construct_directed_pair(int from, int to, int cap) {
            Edge* e1 = new Edge(from, to, cap);
            Edge* e2 = new Edge(to, from, 0);
            e1->reverse = e2;
            e2->reverse = e1;
            return make_pair(e1, e2);
        }
    };

    int n_vertices;
    multimap<int, Edge*> edges;

public:
    explicit EdmondsKarp(int n) : n_vertices(n) {};

    ~EdmondsKarp() {
        for (auto p : edges)
            delete p.second;
    }

    static void breadth_first_search(const multimap<int, Edge*>& edges, vector<Edge*>& predecessors, int n_vertices, int src) {
        predecessors.clear();
        predecessors.resize(n_vertices, nullptr);

        queue<int> q;
        q.push(src);
        while (! q.empty()) {
            int curr = q.front(); q.pop();
            for (auto it = edges.equal_range(curr); it.first != it.second; ++it.first) {
                Edge* e = it.first->second;
                if (predecessors[e->dest] == nullptr and e->dest != src and e->cap > e->flow) {
                    predecessors[e->dest] = e;
                    q.push(e->dest);
                }
            }
        }
    }

    void AddEdge(int from, int to, int cap) {
        auto p = Edge::construct_directed_pair(from, to, cap);
        edges.insert(make_pair(p.first->src, p.first));
        edges.insert(make_pair(p.second->src, p.second));
    }

    int GetMaxFlow(int source, int sink) {
        int flow = 0;
        bool first_iter = true;

        // Predecessors
        vector<Edge*> predecessors;
        do {
            // 1. Run BFS to find the shortest path from source to sink
            breadth_first_search(edges, predecessors, n_vertices, source);

            if (first_iter) {
                if (predecessors[sink] == nullptr) return -1;
                first_iter = false;
            }

            // 2. If an augmenting path was found
            if (predecessors[sink] != nullptr) {
                int df = numeric_limits<int>::max();
                // 2.1. See how much flow we can send
                for (Edge * e = predecessors[sink]; e != nullptr; e = predecessors[e->src])
                    df = min(e->cap - e->flow, df);

                // 2.2. And update edge by that amount
                for (Edge * e = predecessors[sink]; e != nullptr; e = predecessors[e->src]) {
                    e->flow += df;
                    e->reverse->flow -= df;
                }

                flow += df;
            }
        } while (predecessors[sink] != nullptr);

        return flow;
    }
};

class TestCase {
    int grid_size;
    int n_riders;
    int n_nights;

    vector<vector<int>> grid_height;
    vector<pair<int, int>> rider_pos;
    vector<int> nightly_snow_height;

public:
    explicit TestCase(istream& in) {
        in >> grid_size >> n_riders >> n_nights;

        this->grid_height = vector<vector<int>>(grid_size, vector<int>(grid_size));
        for (int row = 0; row < grid_size; ++row) {
            for (int col = 0; col < grid_size; ++col) {
                in >> grid_height[row][col];
            }
        }

        this->rider_pos = vector<pair<int, int>>(n_riders);
        for (int i = 0; i < n_riders; ++i) {
            in >> rider_pos[i].first >> rider_pos[i].second;
        }

        this->nightly_snow_height = vector<int>(n_nights);
        for (int i = 0; i < n_nights; ++i) {
            in >> nightly_snow_height[i];
        }
    }

    /**
     * Translates node's features to its vertex index/id.
     */
    int to_vertex_id(int row, int col, int day, bool offset) {
        return col + row * grid_size + day * (grid_size * grid_size) + (offset ? 1 : 0) * (n_nights + 1) * (grid_size * grid_size);
    }

    /**
     * Converts the given vertex id into a tuple of corresponding <offset, day, row, col>.
     */
    tuple<int, int, int, int> from_vertex_id(int vertex_id) {
        int offset = vertex_id / ((n_nights + 1) * grid_size * grid_size);
        vertex_id -= offset * ((n_nights + 1) * grid_size * grid_size);
        int day = vertex_id / (grid_size * grid_size);
        vertex_id -= day * (grid_size * grid_size);
        int row = vertex_id / grid_size;
        vertex_id -= row * grid_size;
        int col = vertex_id;
        return make_tuple(offset, day, row, col);
    }

    void add_grid_edges(MaxFlowAlgorithm& algo, int day) {
        assert(day >= 0 and day <= n_nights);
        for (int row = 0; row < this->grid_size; ++row) {
            for (int col = 0; col < this->grid_size; ++col) {
                // Edge southwards
                if (row < this->grid_size - 1) {
                    int src = to_vertex_id(row, col, day, false);
                    int dst = to_vertex_id(row + 1, col, day, true);
                    algo.AddEdge(src, dst, 1);
                }

                // Edge northwards
                if (row > 0) {
                    int src = to_vertex_id(row, col, day, false);
                    int dst = to_vertex_id(row - 1, col, day, true);
                    algo.AddEdge(src, dst, 1);
                }

                // Edge eastwards
                if (col < this->grid_size - 1) {
                    int src = to_vertex_id(row, col, day, false);
                    int dst = to_vertex_id(row, col + 1, day, true);
                    algo.AddEdge(src, dst, 1);
                }

                // Edge westwards
                if (col > 0) {
                    int src = to_vertex_id(row, col, day, false);
                    int dst = to_vertex_id(row, col - 1, day, true);
                    algo.AddEdge(src, dst, 1);
                }
            }
        }
    }

    /**
     * Adds the edges between corresponding nodes of different days, depending on
     *  whether that node is survivable.
     * @param algo MaxFlowAlgorithm instance to be modified.
     * @param night index of current night in range [0, n_nights[
     */
    void add_night_edges(MaxFlowAlgorithm& algo, int night) {
        assert(night >= 0 and night <= n_nights - 1);
        for (int row = 0; row < grid_size; ++row) {
            for (int col = 0; col < grid_size; ++col) {

                // Is node survivable?
                if (grid_height[row][col] > nightly_snow_height[night]) {
                    int src = to_vertex_id(row, col, night, true);
                    int dst = to_vertex_id(row, col, night+1, false);
                    algo.AddEdge(src, dst, 1);
                }
            }
        }
    }

    /**
     * Constructs a flow network representing the "Goat Riders" problem.
     * - each day is a repetition of the original grid/network in a different dimension;
     * - different days are joined through a 1-capacity edge if rider could survive that night.
     */
    string solve() {
        int n_vertices = 3 + 2 * (grid_size * grid_size) * (n_nights + 1);
        MaxFlowAlgorithm* max_flow_algo = new PushRelabel(n_vertices);

        int pre_source = n_vertices - 3, source = n_vertices - 2, sink = n_vertices - 1;
        // Add edge from pre_source to source (to control maximum outgoing flow from source)
        max_flow_algo->AddEdge(pre_source, source, n_riders);

        // Add edges from source to first day's nodes
        for (int i = 0; i < grid_size * grid_size; ++i)
            max_flow_algo->AddEdge(source, i, 1);

        // Add edges from last day's nodes to sink
        for (int i = n_vertices - 3 - (grid_size * grid_size); i < n_vertices - 3; ++i)
            max_flow_algo->AddEdge(i, sink, 1);

        // For every day, add edges between neighbouring grid nodes
        for (int i = 0; i < n_nights + 1; ++i)
            add_grid_edges(*max_flow_algo, i);

        // For every night, add edges between analogous survivable nodes
        for (int i = 0; i < n_nights; ++i) {
            add_night_edges(*max_flow_algo, i);
        }

        int max_flow = max_flow_algo->GetMaxFlow(pre_source, sink);
        delete max_flow_algo;
        return to_string(max_flow);
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
