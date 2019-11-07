// Week 4 - Problem C

#include <iostream>
#include <vector>
#include <map>
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

    string solve() {
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
