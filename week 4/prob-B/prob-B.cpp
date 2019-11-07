// Week 4 - Problem B

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <cmath>
#include <cassert>

using namespace std;

struct Edge {
    uint origin, dest;
    double exchange_rate;
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
            in >> e.origin >> e.dest >> e.exchange_rate;
            edges.insert(make_pair(e.origin, e));
        }
    }

    static double exchangeDist(double logDist, double newEdgeDist) {
        return logDist + log(newEdgeDist);
    }

    static bool all_reachable(const vector<bool>& visited) {
        for (uint i = 1; i < visited.size(); ++i) {
            if (! visited[i]) return false;
        }
        return true;
    }

    string solve(uint start_node = 1) {
        // Distances from source node
        vector<double> log_exchange_rate(n_vertices + 1, numeric_limits<double>::max());

        // Visited nodes
        vector<bool> visited(n_vertices + 1, false);

        // NOTE this would be more efficient with a Fibonacci Heap
        typedef pair<double, uint> pi;
        priority_queue<pi, vector<pi>, greater<pi>> pqueue; // priority queue of elements <distance, node>

        // Exchange rate at origin is 1 => log(1) = 0
        pqueue.push(make_pair(0, start_node)); // starts at node 1 with distance 0
        log_exchange_rate[start_node] = 0;

        bool jackpot = false; // When a virtuous exchange loop is found
        while (! pqueue.empty()) {
            auto p = pqueue.top(); pqueue.pop();
            uint node = p.second;
            visited[node] = true;

            for (auto it = edges.equal_range(node); it.first != it.second; ++it.first) {
                Edge e = it.first->second;
                double log_dist = exchangeDist(p.first, e.exchange_rate);

                // !visited[e.dest] and
                if (log_dist < log_exchange_rate[e.dest]) {
                    if (visited[e.dest]) {
                        jackpot = true;
                        continue;
                    }
                    log_exchange_rate[e.dest] = log_dist;
                    pqueue.push(make_pair(log_dist, e.dest));
                    // NOTE We should delete the previous pair containing this node
                }
            }
        }

        if (! visited.back())
            return "impossible";
        else if (jackpot and all_reachable(visited))
            return "Jackpot";
        else
            return to_string(exp(log_exchange_rate.back()));
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
