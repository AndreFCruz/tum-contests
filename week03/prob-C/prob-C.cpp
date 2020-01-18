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

/**
 * Returns indices of a topological order
 * @param dependencies  edges representing dependencies between nodes
 * @param n_vertices    number of vertices/nodes
 * @return a vector in which v[node_id - 1] = node_topological_order
 */
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

    vector<uint> topological_order(n_vertices);
    uint counter = 0;
    while (! s.empty()) {
        uint node = *s.begin();
        s.erase(node);
        topological_order[node-1] = ++counter;

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
    if (! dependencies.empty())
        throw domain_error("provided graph is cyclic");

    return topological_order;
}

struct PairHash {
    int operator()(const pair<uint, uint>& p) const {
        return p.first * 1000 + p.second;   // 1000 is the max number of nodes
    }
};

class TestCase {
    uint n_vertices;    // number of intersections
    multimap<uint, uint> edges;
    unordered_set<pair<uint, uint>, PairHash> two_way_roads;

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

    string solve() {
        multimap<uint, uint> inverted_edges;
        for (const auto& p : edges)
            inverted_edges.insert(make_pair(p.second, p.first));

        vector<uint> topo_order;
        try {
            topo_order = topological_sort(inverted_edges, n_vertices);
        } catch (domain_error& e) {
            // Graph is cyclic
            return "no";
        }

        vector<pair<uint, uint>> new_edges;
        for (const auto& road : two_way_roads) {
            if (topo_order[road.first - 1] < topo_order[road.second - 1]) {
                new_edges.push_back(make_pair(road.first, road.second));
            } else {
                new_edges.push_back(make_pair(road.second, road.first));
            }
            edges.insert(new_edges.back());
            // TODO check if new graph has a cycle ?
        }

        string answer = "yes";
        for (auto p : new_edges)
            answer += "\n" + to_string(p.first) + " " + to_string(p.second);
        return answer;
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