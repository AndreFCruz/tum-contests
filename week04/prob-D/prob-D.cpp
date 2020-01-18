// Week 4 - Problem D

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

class TestCase {
    uint n_tasks;
    multimap<uint, uint> edges;   // map[u] = v iff task u comes before task v
    vector<uint> costs;

public:
    explicit TestCase(istream& in) {
        in >> n_tasks;

        // node 0 serves as an artificial starting node which all tasks depend on
        constexpr uint artificial_start_node = 0;
        costs = vector<uint>(n_tasks + 1);
        costs[artificial_start_node] = 0;

        uint cost, n_successors, dependent_node;
        for (uint i = 1; i <= n_tasks; ++i) {
            in >> cost >> n_successors;

            edges.insert(make_pair(artificial_start_node, i));   // artificial edge from 0 to i
            costs[i] = cost;

            for (uint j = 0; j < n_successors; ++j) {
                in >> dependent_node;
                edges.insert(make_pair(i, dependent_node));
            }
        }
    }

    // NOTE Assumes graph is a DAG
    static uint max_path_from(const multimap<uint, uint>& edges, const vector<uint>& costs, uint from_node, unordered_map<uint, uint>& cache) {
        uint cost = 0;
        for (auto it = edges.equal_range(from_node); it.first != it.second; ++it.first) {
            if (cache.count(it.first->second) > 0)
                cost = max(cost, cache[it.first->second]);
            else
                cost = max(cost, max_path_from(edges, costs, it.first->second, cache));
        }
        cost += costs[from_node];
        cache[from_node] = cost;
        return cost;
    }

    string solve() {
        // Find critical path (longest path) from source node 0 (to any node)
        unordered_map<uint, uint> cache;
        uint critical_path_cost = max_path_from(edges, costs, 0, cache);
        return to_string(critical_path_cost);
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
