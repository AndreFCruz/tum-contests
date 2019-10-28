#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <map>
#include <unordered_set>
#include <exception>

using namespace std;

/**
 * Union-find (a.k.a. disjoint-set or merge-find set) tracks a set of
 *  elements partitioned into a number of disjoint subsets with near
 *  constant-time operations (order of inverse Ackermann function).
 */
class UnionFind {
private:
    size_t n;
    vector<int> parent;
    vector<int> size;

public:
    UnionFind(size_t n) : n(n) {
        this->size = vector<int>(n, 1);
        this->parent = vector<int>(n);

        int i = 0;
        generate(parent.begin(), parent.end(), [&]() {
            return i++;
        });

    }

    /**
     * Follows the chain of parent pointers from _a_ up the tree until
     *  it reaches a root element, whose parent is itself (this is the
     *  representative element of the set).
     */
    int find(int a) {
        // Find root
        int root = a;
        while (parent[root] != root)
            root = parent[root];

        // Compress path
        int current = a, next_elem;
        while (current != root) {
            next_elem = parent[current];
            parent[current] = root;
            current = next_elem;
        }

        return root;
    }

    /**
     * Uses find to determine the roots of a and b, and combines roots if they are distinct.
     * (attach root of smaller set to root of the larger set; if done without taking size
     * into account the tree can degenerate to O(n) height/depth).
     */
    int union_join(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) // a and b are already merged
            return a;

        // Weighted union: update smaller component
        int a_size = size[a], b_size = size[b];
        if (a_size < b_size)
            swap(a, b);

        parent[b] = a;
        // Update size accordingly
        size[a] = a_size + b_size;
        return a;
    }

    int size_of(int idx) {
        return size[idx];
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }
};

class TestCase {
    const char ANTIPATHY = 'A'; // Antipathy
    const char FRIENDSHIP = 'F';  // Friendship

    int n_countries;
    int n_interactions;

    UnionFind * uf = nullptr;
    vector<int> antipathy;

public:

    TestCase(istream & in) {
        char relation;
        int c1, c2;
        in >> n_countries >> n_interactions;

        this->uf = new UnionFind(n_countries + 1);
        this->antipathy = vector<int>(n_countries + 1, 0);

        for (int i = 0; i < n_interactions; ++i) {
            in >> relation >> c1 >> c2;

            // Identify all countries by their parent's id
            // (so when new friends/enemies are added they ripple out to the whole group)
            int c1_parent = uf->find(c1), c2_parent = uf->find(c2);
            if (relation == FRIENDSHIP) {
                uf->union_join(c1_parent, c2_parent);
                if (antipathy[c1_parent] == 0 and antipathy[c2_parent] == 0)
                    ; // Nothing to do
                else if (antipathy[c1_parent] != 0 and antipathy[c2_parent] == 0)
                    antipathy[c2_parent] = antipathy[c1_parent];
                else if (antipathy[c1_parent] == 0 and antipathy[c2_parent] != 0)
                    antipathy[c1_parent] = antipathy[c2_parent];
                else
                    uf->union_join(antipathy[c1_parent], antipathy[c2_parent]);

            } else if (relation == ANTIPATHY) {
                if (antipathy[c1_parent] == 0 and antipathy[c2_parent] == 0) {
                    // Both still don't hate anyone
                    antipathy[c1_parent] = c2_parent;
                    antipathy[c2_parent] = c1_parent;
                }
                else if (antipathy[c1_parent] != 0 and antipathy[c2_parent] == 0) {
                    // c1's group hates someone and c2's group doesn't, join c1's enemies with c2's group
                    uf->union_join(antipathy[c1_parent], c2_parent);
                    antipathy[c2_parent] = c1_parent;
                }
                else if (antipathy[c1_parent] == 0 and antipathy[c2_parent] != 0) {
                    // reverse of previous condition
                    uf->union_join(antipathy[c2_parent], c1_parent);
                    antipathy[c1_parent] = c2_parent;
                }
                else {
                    // Both already hate someone
                    uf->union_join(c1_parent, antipathy[c2_parent]); // c1 is now friends with c2's enemies
                    uf->union_join(c2_parent, antipathy[c1_parent]); // c2 is now friends with c1's enemies
                }

            } else
                throw(invalid_argument("Relation must be one of {F, A}"));

        }

    }

    string solve() {
        // Check size of alliance containing country with id 1
        return uf->size_of(uf->find(1)) > (n_countries / 2) ? "yes" : "no";
    }

    ~TestCase() {
        delete this->uf;
    }

};

int main() {
    std::ios_base::sync_with_stdio(false); // for better performance

    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i)
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;

    return EXIT_SUCCESS;
}
