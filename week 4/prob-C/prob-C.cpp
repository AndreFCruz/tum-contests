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

    Edge reverse() {
        Edge reverse;
        reverse.origin = this->dest;
        reverse.dest = this->origin;
        reverse.cost = this->cost;
        return reverse;
    }
};

struct EdgeCmp {
    bool operator()(const Edge& e1, const Edge& e2) {
        if (e1.cost != e2.cost) return e1.cost < e2.cost;
        return e1.dest != e2.dest ? e1.dest < e2.dest : e1.origin < e2.origin;
    }
};

class TestCase {
    uint n_cities;  // n
    uint n_roads;   // m
    uint n_markets; // s
    uint start_node;    // a
    uint end_node;      // b

    // Roads between cities
    multimap<uint, Edge, EdgeCmp> edges;

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
        uint n = sprintf(formatted, "%02u:%02u", hours, mins);
        formatted[n] = 0;
        return string(formatted);
    }

    string solve() {
        // Do Dijkstra shortest-path
        // maintain 2 distance vectors: one as normal, and another for paths which have gone through a supermarket
        // on every vertex that has a supermarket, visit it
        // when comparing distances:
        // if node has supermarket: market_dist[n] = min(market_dist[n], regular_dist[n] + market.time)
        // else: market_dist[n] = market_dist[parent] != inf ? market_dist[parent] + edge.cost : inf
        // regular_dist is updates as normal

        return "impossibruu";
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
