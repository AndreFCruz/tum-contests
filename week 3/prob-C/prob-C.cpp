// Week 3 - Problem C

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <exception>

using namespace std;

typedef unsigned int uint;

// TODO Check if a cycle is reached, if so throw an exception
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

    // Dependencies should be empty by now
    if (! dependencies.empty()) // TODO CHECK
        throw domain_error("provided graph is cyclic");

    return topological_order;
}

class TestCase {
    uint n_vertices;    // number of intersections
    multimap<uint, uint> edges;
    unordered_set<pair<uint, uint>> two_way_roads;

public:
    explicit TestCase(istream& in) {
        uint n_directed;    // number of one-way roads
        uint n_undirected;  // number of two-way roads
        in >> this->n_vertices >> n_directed >> n_undirected;

        uint n1, n2;
        for (uint i = 0; i < n_directed; ++i) {
            in >> n1 >> n2;
            // directed road from intersection n1 to intersection n2
            edges.insert(make_pair(n1, n2));
        }

        for (uint i = 0; i < n_undirected; ++i) {
            in >> n1 >> n2;
            // undirected road from intersection n1 to intersection n2
            if (n1 > n2) swap(n1, n2);
            two_way_roads.insert(make_pair(n1, n2));
        }

        assert(edges.size() == n_directed);
        assert(two_way_roads.size() == n_undirected);
    }

    string solve() const {
        auto topo_sort = topological_sort(edges, n_vertices); // TODO catch exception

        return "TODO";
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