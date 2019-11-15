#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

using namespace std;

// Push-Relabel Max-Flow Algorithm from https://codeforces.com/blog/entry/14378
/*
    Implementation of highest-label push-relabel maximum flow
    with gap relabeling heuristic.

    Running time:
        O(|V|^2|E|^{1/2})

    Usage:
        - add edges by AddEdge()
        - GetMaxFlow(s, t) returns the maximum flow from s to t

    Input:
        - graph, constructed using AddEdge()
        - (s, t), (source, sink)

    Output:
        - maximum flow value
*/

template <class T> struct Edge {
    int from, to, index;
    T cap, flow;

    Edge(int from, int to, T cap, T flow, int index): from(from), to(to), cap(cap), flow(flow), index(index) {}
};

template <class T> struct PushRelabel {
    int n;
    vector <vector <Edge <T>>> adj;
    vector <T> excess;
    vector <int> dist, count;
    vector <bool> active;
    vector <vector <int>> B;
    int b;
    queue <int> Q;

    PushRelabel (int n): n(n), adj(n) {}

    void AddEdge (int from, int to, int cap) {
        adj[from].push_back(Edge <T>(from, to, cap, 0, adj[to].size()));
        if (from == to) {
            adj[from].back().index++;
        }
        adj[to].push_back(Edge <T>(to, from, 0, 0, adj[from].size() - 1));

    }

    void Enqueue (int v) {
        if (!active[v] && excess[v] > 0 && dist[v] < n) {
            active[v] = true;
            B[dist[v]].push_back(v);
            b = max(b, dist[v]);
        }
    }

    void Push (Edge <T> &e) {
        T amt = min(excess[e.from], e.cap - e.flow);
        if (dist[e.from] == dist[e.to] + 1 && amt > T(0)) {
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

    T GetMaxFlow (int s, int t) {
        dist = vector <int>(n, 0), excess = vector<T>(n, 0), count = vector <int>(n + 1, 0), active = vector <bool>(n, false), B = vector <vector <int>>(n), b = 0;

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
    int to_vertex_id(int row, int col, int day) {
        return col + row * grid_size + day * (grid_size * grid_size);
    }

    void add_grid_edges(PushRelabel<int>& pr, int day) {
        for (int row = 0; row < this->grid_size; ++row) {
            for (int col = 0; col < this->grid_size; ++col) {
                // Edge downwards
                if (row < this->grid_size - 1) {
                    int a = to_vertex_id(row, col, day);
                    int b = to_vertex_id(row + 1, col, day);
                    pr.AddEdge(a, b, 1);
                    pr.AddEdge(b, a, 1);
                }

                // Edge rightwards
                if (col < this->grid_size - 1) {
                    int a = to_vertex_id(row, col, day);
                    int b = to_vertex_id(row, col + 1, day);
                    pr.AddEdge(a, b, 1);
                    pr.AddEdge(b, a, 1);
                }
            }
        }
    }

    /**
     * Adds the edges between corresponding nodes of different days, depending on
     *  whether that node is survivable.
     * @param pr PushRelabel instance to be modified.
     * @param night index of current night in range [0, n_nights[
     */
    void add_night_edges(PushRelabel<int>& pr, int night) {
        for (int row = 0; row < grid_size; ++row) {
            for (int col = 0; col < grid_size; ++col) {

                // Is node survivable?
                if (grid_height[row][col] > nightly_snow_height[night]) {
                    pr.AddEdge(to_vertex_id(row, col, night), to_vertex_id(row, col, night+1), 1);
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
        int n_vertices = 3 + (grid_size * grid_size) * (n_nights + 1);
        PushRelabel<int> pr(n_vertices);

        int pre_source = n_vertices - 3, source = n_vertices - 2, sink = n_vertices - 1;
        // Add edge from pre_source to source (to control maximum outgoing flow from source)
        pr.AddEdge(pre_source, source, n_riders);

        // Add edges from source to first day's nodes
        for (int i = 0; i < grid_size * grid_size; ++i)
            pr.AddEdge(source, i, 1);

        // Add edges from lat day's nodes to sink
        for (int i = grid_size * grid_size * n_nights; i < grid_size * grid_size * (n_nights + 1); ++i)
            pr.AddEdge(i, sink, 1);

        // For every day, add edges between neighbouring grid nodes
        for (int i = 0; i < n_nights + 1; ++i)
            add_grid_edges(pr, i);

        // For every night, add edges between analogous survivable nodes
        for (int i = 0; i < n_nights; ++i) {
            add_night_edges(pr, i);
        }

        return to_string(pr.GetMaxFlow(pre_source, sink));
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
