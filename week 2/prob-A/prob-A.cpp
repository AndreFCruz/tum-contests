#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <string>

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

    vector<int> uf;

public:
    UnionFind(size_t n) : n(n) {
        this->size = vector<int>(n, 1);
        this->parent = vector<int>(n);

        generate(parent.begin(), parent.end(), []() {
            static int i = 0;
            return i++;
        });

        this->uf = vector<int>(n, -1);
    }

    /**
     * Follows the chain of parent pointers from _a_ up the tree until
     *  it reaches a root element, whose parent is itself (this is the
     *  representative element of the set).
     */
     int find(int x) {
        int r = x;

        /* Find root */
        while(uf[r] >= 0)
            r = uf[r];

        /* Compress path to root */
        while(uf[x] >= 0) {
            int tmp = uf[x];
            uf[x] = r;
            x = tmp;
        }

        return r;
     }
//    int find(int a) {
//        cout << "Called find with a: " << a << endl;
//
//        // Find root
//        int root = a;
//        while (parent[root] != root) {
//            root = parent[root];
//            assert(parent[root] < parent.size());
//        }
//
//        // NOTE Second Try
////        int root = a, p;
////        while (true) {
////            p = parent[root];
////            if (p == root)
////                break;
////            root = p;
////        }
//
//        // Compress path
//        int current = a, next_elem;
//        while (current != root) {
//            next_elem = parent[current];
//            parent[current] = root;
//            current = next_elem;
//            assert(parent[current] < parent.size());
//        }
//
//        return root;
//
////        if (parent[a] != a)
////            parent[a] = find(parent[a]);
////        return parent[a];
//    }

    /**
     * Uses find to determine the roots of a and b, and combines roots if they are distinct.
     * (attach root of smaller set to root of the larger set; if done without taking size
     * into account the tree can degenerate to O(n) height/depth).
     */
     void union_join(int x, int y) {
        x = find(x);
        y = find(y);

        if(x != y) {
            if(uf[x] < uf[y]) {
                uf[x] += uf[y];
                uf[y] = x;
            }
            else {
                uf[y] += uf[x];
                uf[x] = y;
            }
        }
     }
//    int union_join(int a, int b) {
//        a = find(a);
//        b = find(b);
//        if (a == b) // a and b are already merged
//            return a;
//
//        // Weighted union: update smaller component
//        int a_size = size[a], b_size = size[b];
//        if (a_size < b_size)
//            swap(a, b);
//
//        parent[b] = a;
//        // Update size accordingly
//        size[a] = a_size + b_size;
//        assert(parent[a] < parent.size() and parent[b] < parent.size());
//        return a;
//    }

    /**
     * Compress paths from each node to its parent.
     */
    void compress_paths() {
        for (size_t i = 0; i < parent.size(); ++i)
            parent[i] = find(i);
    }

};

class NotablePerson {
public:
    int id;
    int money = 0;
    unordered_set<int> relations;
    int marriage = -1;  // marriage is a subset of this person's relations

    NotablePerson(int id) : id(id) {}
};


class TestCase {
private:
    vector<NotablePerson> people;

public:
    TestCase(istream & in) {
        int a;  // num. notable people
        int b;  // num. family relations
        int c;  // num. marriages
        in >> a >> b >> c;

        // Money
        int n1, n2;
        for (int i = 0; i < a - 1; ++i) {
            people.push_back(NotablePerson(i + 1));
            in >> n1;
            people.back().money = n1;
        }
        people.push_back(NotablePerson(a)); // Insert Jakob Fugger

        // Family relations
        for (int i = 0; i < b; ++i) {
            in >> n1 >> n2;
            people[n1 - 1].relations.insert(n2);
            people[n2 - 1].relations.insert(n1);

            assert(people[n1 - 1].id == n1 and people[n2 - 1].id == n2);
        }

        // Marriages
        for (int i = 0; i < c; ++i) {
            in >> n1 >> n2;
            people[n1 - 1].marriage = n2;
            people[n2 - 1].marriage = n1;

            people[n1 - 1].relations.insert(n2);
            people[n2 - 1].relations.insert(n1);
        }
    }

    string solve() {
        // Construct UnionFind
        UnionFind uf(people.size());
        for (auto p1 : people) {
            for (auto p2 : p1.relations) {
                uf.union_join(p1.id - 1, p2 - 1);
                assert(people[p1.id - 1].id == p1.id and people[p2 - 1].id == p2);
            }
        }

        vector<int> choices(people.size());
        generate(choices.begin(), choices.end(), []() {static int i = 0; return i++;});

        int jakob_idx = people.size() - 1;
        // Remove if belongs to the same set as Jakob, or if married
        auto end_it = remove_if(choices.begin(), choices.end(), [&](int n) {
            return uf.find(n) == uf.find(jakob_idx) or people[n].marriage != -1;
        });
        choices.erase(end_it, choices.end());
        if (choices.empty())
            return "impossible";

        // Sort according to wealth
        sort(choices.begin(), choices.end(), [&](int a, int b) {
            return people[a].money < people[b].money;
        });

        return to_string(people[choices.back()].money);
    }

};


int main() {
    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}