#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <algorithm>
#include <limits>
#include <queue>
#include <cmath>

#define MAX_ERROR   1e-8

using namespace std;


// Push-Relabel Max-Flow Algorithm (adapted) from https://codeforces.com/blog/entry/14378
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

    void AddEdge (int from, int to, T cap) {
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
        for (auto e: adj[v]) if (e.cap - e.flow > MAX_ERROR) {
                dist[v] = min(dist[v], dist[e.to] + 1);
            }
        count[dist[v]]++;
        Enqueue(v);
    }

    void Discharge(int v) {
        for (auto &e: adj[v]) {
            if (excess[v] > MAX_ERROR) {
                Push(e);
            } else {
                break;
            }
        }

        if (excess[v] > MAX_ERROR) {
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
    int n_valves;
    int n_pipes;

    PushRelabel<double> * pr = nullptr;

public:
    explicit TestCase(istream& in) {
        in >> n_valves >> n_pipes;
        pr = new PushRelabel<double>(n_valves + 1);

        for (int i = 0; i < n_pipes; ++i) {
            int v_a, v_b, k; double x;
            in >> v_a >> v_b >> k >> x;

            double area = calcArea(k, x);
            pr->AddEdge(v_a, v_b, area);
            pr->AddEdge(v_b, v_a, area);
        }
    }

    ~TestCase() {
        delete this->pr;
    }

    // Formula from: https://www.mathopenref.com/polygonregulararea.html
    static double calcArea(int n_sides, double side_len) {
        if (0 == n_sides)
            return M_PI * side_len * side_len;
        double k = static_cast<double> (n_sides);
        return k * side_len * side_len / (4. * tan(M_PI / k));
    }

    void solve() {
        double res = pr->GetMaxFlow(1, this->n_valves);
        if (res < MAX_ERROR)
            cout << "impossible";
        else
            cout << setprecision(16) << res;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": ";
        TestCase(cin).solve();
        cout << endl;
    }

    return EXIT_SUCCESS;
}
