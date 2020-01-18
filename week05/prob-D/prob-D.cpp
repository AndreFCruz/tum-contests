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
    int n_vertices;
    int n_edges;

    vector<int> wins;
    multimap<int, int> connections;

    int source, sink;

public:
    explicit TestCase(istream& in) {
        in >> n_vertices >> n_edges;
        this->source = 1;
        this->sink = this->n_vertices;

        this->wins = vector<int>(this->n_vertices + 1);
        for (int i = 1; i <= this->n_vertices; ++i)
            in >> this->wins[i];

        int a, b;
        for (int i = 0; i < n_edges; ++i) {
            in >> a >> b;
            if (a > b) swap(a, b);
            connections.insert(make_pair(a, b));
        }

        this->source = 0;
        this->sink = this->n_vertices + 1;
        this->n_vertices += 2;
    }

    /**
     * For each team, t, find out if they can win the championship:
     *  if t wins all its games, can other teams play between themselves in a way that leaves
     *  no team with more wins than t ? (is there a valid arrangement of wins that leaves t as
     *  a winner or tied?)
     */
    string solve() {
        string answer = "";

        // For all nodes representing teams (all other than source and sink)
        for (int team = 1; team < this->n_vertices - 1; ++team) {

            // Count maximum possible number of wins for this team
            int max_wins = this->wins[team];
            for (auto p : connections) {
                if (p.first == team or p.second == team)
                    max_wins += 1;
            }

            PushRelabel<int> push_relabel(this->n_vertices);
            vector<int> tmp_wins(this->n_vertices - 1, 0);

            for (auto p : connections) {
                // Ignore team's games as this will all be wins for team
                if (p.first == team or p.second == team)
                    continue;
                tmp_wins[p.first] += 1;
                push_relabel.AddEdge(p.first, p.second, 1);
            }

            // Each edge represents a game (flow represents a win/point)
            for (int i = 1; i < this->n_vertices - 1; ++i) {
                if (i == team) continue;
                // Add edge from source to team
                push_relabel.AddEdge(this->source, i, wins[i] + tmp_wins[i]);

                // Add edge from team to sink
                push_relabel.AddEdge(i, this->sink, max_wins);
                // capacity == max_wins, because we want to see if we can get as many wins with all other teams
            }

            push_relabel.GetMaxFlow(this->source, this->sink);

            // If there's no excess left on any node, than configuration is valid (all flow could be drained)
            bool yes = true;
            for (int i = 0; i < this->n_vertices - 1; ++i) {
                if (push_relabel.excess[i] != 0) {
                    yes = false;
                    break;
                }
            }

            answer += yes ? "yes " : "no ";
        }

        return answer;
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
