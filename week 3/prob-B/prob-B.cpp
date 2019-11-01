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

    // Save nodes which have no predecessors/dependencies
    for (const auto& p : dependencies)
        s.erase(p.first);

    // Build reverse dependencies for faster lookups of nodes
    multimap<uint, uint> r_dependencies;
    for (const auto& p: dependencies)
        r_dependencies.insert(make_pair(p.second, p.first));

    vector<uint> topological_order;
    while (! s.empty()) {
        uint node = *s.begin();
        s.erase(node);
        topological_order.push_back(node);

        // Remove node from Graph (all dependencies on this node)
        //  all dependencies[x, node],      x in nodes
        //  all r_dependencies[node, x],    x in nodes
        auto it = r_dependencies.equal_range(node);
        while (it.first != it.second) {
            uint src = it.first->first, dest = it.first->second;
            assert(src == node);

            // For all of dest's dependencies, remove the one on node src
            bool check_deleted = false;
            for (auto it_pair = dependencies.equal_range(dest); it_pair.first != it_pair.second; ++it_pair.first) {
                if (it_pair.first->first == dest and it_pair.first->second == src) {
                    check_deleted = true;
                    dependencies.erase(it_pair.first); break;
                }
            }
            assert(check_deleted);

            // Remove this edge from r_dependencies
            r_dependencies.erase(it.first++);

            // If dest node has no predecessors (no incoming edges), add to set s
            if (dependencies.count(dest) == 0)
                s.insert(dest);
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