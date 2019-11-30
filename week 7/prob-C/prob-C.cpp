#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <unordered_set>

#define CONNECTED       1
#define NOT_CONNECTED   0
#define CUT             -1

using namespace std;

class TestCase {
    int n_cities;
    multimap<int, int> edges;

public:
    explicit TestCase(istream& in) {
        in >> n_cities;

        for (int c = 1; c <= n_cities; ++c) {
            int n_links, link;
            in >> n_links;
            for (int i = 0; i < n_links; ++i) {
                in >> link;
                edges.insert(make_pair(c - 1, link - 1));
                edges.insert(make_pair(link - 1, c - 1));
            }
        }
    }

    /**
     * Deterministic loacl search for Max-Cut approximation
     * https://en.wikipedia.org/wiki/Maximum_cut#Approximation_algorithms
     * @param cities whether each city belongs to the 1st set or not
     * @return the new solution value
     */

    /**
     *
     * @param cities_first
     * @param cities_second
     * @param connections
     * @param curr_value
     * @return
     */
    int local_search(
            unordered_set<int>& cities_first,
            unordered_set<int>& cities_second,
            vector<vector<int>>& connections,
            int curr_value)
    {
        // Get node with highest number of connections in 1st set
        int node_to_move = -1, max_connections_cut = 0;
        for (int node : cities_first) {

            int connections_cut = 0; // if this node is added, what's the balance of connections cut (the improvement to the solution)
            for (int node_scnd : cities_second) {
                if (connections[node][node_scnd] == CUT)
                    connections_cut -= 1;
                assert(connections[node][node_scnd] != CONNECTED);
            }

            for (int node_frst : cities_first) {
                if (node == node_frst) continue;
                if (connections[node][node_frst] == CONNECTED)
                    connections_cut += 1;
                assert(connections[node][node_frst] != CUT);
            }

            if (connections_cut > max_connections_cut) {
                max_connections_cut = connections_cut;
                node_to_move = node;
                break; // break at first improved solution found? or search for best solution?
            }
        }
        if (max_connections_cut <= 0) return curr_value;

        // Move selected node to second set
        cities_first.erase(node_to_move);
        cities_second.insert(node_to_move);
        for (int other_node = 0; other_node < n_cities; ++other_node) {
            if (other_node == node_to_move) continue;
            if (connections[node_to_move][other_node] == NOT_CONNECTED) continue;

            int new_connection_state = cities_first.count(other_node) != 0 ? CUT : CONNECTED;
            connections[node_to_move][other_node] = new_connection_state;
            connections[other_node][node_to_move] = new_connection_state;
        }

        return curr_value + max_connections_cut;
    }

    void solve() {
        unordered_set<int> cities_first;
        for (int i = 0; i < n_cities; ++i)
            cities_first.insert(i);
        unordered_set<int> cities_second;

        // Build connections matrix
        vector<vector<int>> connections(n_cities, vector<int>(n_cities, NOT_CONNECTED));
        for (auto e : edges)
            connections[e.first][e.second] = CONNECTED;

        int curr_sol, new_sol = 0;
        do {
            curr_sol = new_sol;
            new_sol = local_search(cities_first, cities_second, connections, 0);
        } while (new_sol < curr_sol);

        // Print solution
        for (int n : cities_first)
            cout << n + 1 << " ";
        cout << endl;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ":" << endl;
        TestCase(cin).solve();
    }

    return EXIT_SUCCESS;
}
