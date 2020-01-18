// Week 3 - Problem B

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <stack>

using namespace std;

typedef unsigned int uint;

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

    /**
     * Checks whether there is a conflict when removing/maintaining the respective nodes.
     * Searches the graph starting from all nodes to be kept, if any node to be removed is
     *  reached through dependency relations, then there is a conflict.
     * @return whether there's a conflict on this operation.
     */
    bool check_conflict() const {
        stack<uint> s;
        for (uint n : to_keep)
            s.push(n);

        unordered_set<uint> to_remove;
        for (uint n : this->to_remove)
            to_remove.insert(n);

        unordered_set<uint> visited;
        while (! s.empty()) {
            uint node = s.top(); s.pop();
            // If node is marked to be removed, conflict was found
            if (to_remove.count(node) > 0)
                return true;

            // If node was not visited
            if (visited.count(node) == 0) {
                visited.insert(node);
                // Visit all of this node's dependencies
                for (auto it = dependencies.equal_range(node); it.first != it.second; ++it.first)
                    s.push(it.first->second);
            }
        }

        return false;
    }

    string solve() const {
        return this->check_conflict() ? "conflict" : "ok";
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