// Week 3 - Problem B
// Topological sort

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <unordered_set>

using namespace std;

typedef unsigned int uint;

vector<uint> topological_sort(multimap<uint, uint> dependencies, uint n_vertices) {
    unordered_set<uint> s;
    for (uint i = 1; i <= n_vertices; ++i)
        s.insert(i);

    // Save nodes which have no predecessors (no incoming edges)
    for (const auto& p : dependencies)
        s.erase(p.second);

    // Build reverse dependencies for performance
    multimap<uint, uint> r_dependencies;
    for (const auto& p: dependencies)
        r_dependencies.insert(make_pair(p.second, p.first));

    vector<uint> topological_order;
    while (! s.empty()) {
        uint node = *s.begin();
        s.erase(node);
        topological_order.push_back(node);

        // Remove node from Graph (remove all outgoing edges)
        auto it = dependencies.equal_range(node);
        while (it.first != it.second) {
            uint src = it.first->first, dest = it.first->second;

            // If dest node has no predecessors (no incoming edges), add to set s
            if (r_dependencies.count(dest) > 0) {
                s.insert(dest);
            }

            // Remove reverse dependencies
            for (auto r_it = r_dependencies.equal_range(dest); r_it.first != r_it.second; ++r_it.first) {
                // If this is the reverse relation of src->dest (dest->src), delete
                if (r_it.first->first == it.first->second and r_it.first->second == it.first->first) {
                    r_dependencies.erase(r_it.first); break;
                }
            }

            // Remove this edge from the graph
            dependencies.erase(it.first++);
        }
    }

    return topological_order;
}

class TestCase {
    uint n_packages;
    vector<uint> to_keep;
    vector<uint> to_remove;
    multimap<uint, uint> dependencies;

public:
    TestCase(istream& in) {
        uint n_keep, n_remove, n_dependencies;
        in >> this->n_packages >> n_keep >> n_remove >> n_dependencies;

        uint n;
        // Packages to keep
        for (uint i = 0; i < n_keep; ++i) {
            in >> n;
            to_keep.push_back(n);
        }

        // Packages to remove
        for (uint i = 0; i < n_remove; ++i) {
            in >> n;
            to_remove.push_back(n);
        }

        uint u, d;
        // Dependencies
        for (uint i = 0; i < n_dependencies; ++i) {
            in >> u >> d;
            // package u depends on package d
            dependencies.insert(make_pair(u, d));
        }
    }

    string solve() {
        auto topo_sort = topological_sort(this->dependencies, this->n_packages);
        // TODO
    }
};

int main() {
    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}