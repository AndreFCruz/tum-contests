// Week 4 - Problem B

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

struct Edge {
    uint origin, dest;
    double exchange_rate;
};

class TestCase {
    uint n_vertices;
    vector<Edge> edges;

public:
    TestCase(istream& in) {
        uint n_edges;
        in >> n_vertices >> n_edges;
        for (uint i = 0; i < n_edges; ++i) {
            Edge e;
            in >> e.origin >> e.dest >> e.exchange_rate;
            edges.push_back(e);
        }
    }

    static double exchangeDist(double logDist, double newEdgeDist) {
        return logDist + log(newEdgeDist);
    }

//    string solve(uint start_node = 1) {
//        // Distances from source node
//        vector<double> log_exchange_rate(n_vertices + 1, numeric_limits<double>::max());
//
//        // Visited nodes
//        vector<bool> visited(n_vertices + 1, false);
//
//        // NOTE this would be more efficient with a Fibonacci Heap
//        typedef pair<double, uint> pi;
//        priority_queue<pi, vector<pi>, greater<pi>> pqueue; // priority queue of elements <distance, node>
//
//        // Exchange rate at origin is 1 => log(1) = 0
//        pqueue.push(make_pair(0, start_node)); // starts at node 1 with distance 0
//        log_exchange_rate[start_node] = 0;
//
//        while (! pqueue.empty()) {
//            auto p = pqueue.top(); pqueue.pop();
//            uint node = p.second;
//            visited[node] = true;
//
//            for (auto it = edges.equal_range(node); it.first != it.second; ++it.first) {
//                Edge e = it.first->second;
//                double log_dist = exchangeDist(p.first, e.exchange_rate);
//
//                // !visited[e.dest] and
//                if (log_dist < log_exchange_rate[e.dest]) {
//                    if (visited[e.dest]) return "Jackpot"; // virtuous exchange cycle
//                    log_exchange_rate[e.dest] = log_dist;
//                    pqueue.push(make_pair(log_dist, e.dest));
//                    // NOTE We should delete the previous pair containing this node
//                }
//            }
//        }
//
//        return visited.back() ? to_string(exp(log_exchange_rate.back())) : "impossible";
//    }


    /**
     * Solves this Test Case using the Bellman-Ford algorithm
     * @param start_node where to start graph traversal
     * @return problem's solution
     */
    string solve(uint start_node = 1) {
        vector<double> log_exchange_rate(this->n_vertices + 1, numeric_limits<double>::max());
        vector<uint> predecessor(this->n_vertices + 1, 0);

        // Exchange rate at origin is 1 => log(1) = 0
        log_exchange_rate[start_node] = 0;

        // Relax edges repeatedly
        for (uint i = 1; i <= this->n_vertices - 1; ++i) {
            for (const Edge& e : edges) {
                double log_dist = exchangeDist(log_exchange_rate[e.origin], e.exchange_rate);
                if (log_dist < log_exchange_rate[e.dest]) {
                    log_exchange_rate[e.dest] = log_dist;
                    predecessor[e.dest] = e.origin;
                }
            }
        }

        // Check for negative-weight cycles, return "Jackpot" if one exists
        for (const Edge& e : edges) {
            double log_dist = exchangeDist(log_exchange_rate[e.origin], e.exchange_rate);
            if (log_dist < log_exchange_rate[e.dest])
                return "Jackpot";
        }

        return predecessor.back() != 0 ? to_string(exp(log_exchange_rate.back())) : "impossible";
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
