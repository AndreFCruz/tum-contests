#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <map>
#include <unordered_set>

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

//    /**
//     * Returns all members of the graph/group containing the given idx;
//     */
//    vector<int> group_containing(int idx) {
//        vector<int> group;
//        int idx_parent = find(idx);
//        for (int i = 0; i < parent.size(); ++i) {
//            if (parent[i] == idx_parent)
//                group.push_back(i);
//        }
//
//        assert(group.size() == size[idx]);
//        return group;
//    }
};

class TestCase {
    const char ANTIPATHY = 'A'; // Antipathy
    const char FRIENDSHIP = 'F';  // Friendship

    int n_countries;
    int n_interactions;

    multimap<int, int> friendship;
    multimap<int, int> antipathy;

    UnionFind * uf = nullptr;

public:
    TestCase(istream & in) {
        char relation;
        int c1, c2;
        in >> n_countries >> n_interactions;
        for (int i = 0; i < n_interactions; ++i) {
            in >> relation >> c1 >> c2;

            if (relation == ANTIPATHY) {
                // Antipathy is mutual
                antipathy.insert(make_pair(c1, c2));
                antipathy.insert(make_pair(c2, c1));
            } else if (relation == FRIENDSHIP) {
                // Friendship is mutual
                friendship.insert(make_pair(c1, c2));
                friendship.insert(make_pair(c2, c1));
            }
        }

        uf = make_union_find();
    }

    UnionFind * make_union_find() {
        auto union_find = new UnionFind(n_countries);

        size_t prev_alliances, curr_alliances = get_num_alliances(*union_find);
        do {
            prev_alliances = curr_alliances;

            // Friendship and hatred relations
            apply_friends_of_friends(*union_find);

            // Apply "a common cause unites people"
            apply_common_cause(*union_find);

            // Apply "an alliance has common enemies"
            apply_common_enemies(*union_find);

            curr_alliances = get_num_alliances(*union_find);
        } while (prev_alliances != curr_alliances);

        return union_find;
    }

    /**
     * Applies rule "friends of my friends are my friends as well",
     * by connecting all friends in a graph.
     */
    void apply_friends_of_friends(UnionFind & uf) {
        for (auto p1 : friendship) {
            int country1 = p1.first;
            int country2 = p1.second;

            uf.union_join(country1 - 1, country2 - 1);
        }
    }

    /**
     * Applies rule "a common cause unites people".
     * If x hates z and y hates z, then x and y form an alliance.
     */
    void apply_common_cause(UnionFind & uf) {
        unordered_set<int> visited; // HashSet
        for (auto p1 : antipathy) {
            int hatee = p1.first;
            if (visited.count(hatee) > 0)
                continue;

            visited.insert(hatee);
            // Get everyone that hates p1 (equal to everyone that p1 hates)
            auto lb = antipathy.lower_bound(hatee), ub = antipathy.upper_bound(hatee);
            int c1, c2;
            while (lb != ub and lb != antipathy.end()) {
                assert(lb->first == hatee); // hatee is the common key
                c1 = lb->second;
                ++lb;
                if (not (lb != ub and lb != antipathy.end()))
                    break;
                c2 = lb->second;
                uf.union_join(c1 - 1, c2 - 1);
            }
        }
    }

    /**
     * Applies rule "an alliance has common enemies".
     */
    void apply_common_enemies(UnionFind & uf) {
        map<int, vector<int>> alliances;
        for (int i = 0; i < n_countries; ++i)
            alliances[uf.find(i)].push_back(i);

        for (const auto & p : alliances) {
            vector<int> hated;
            // Get all hated by all countries in this alliance
            for (int ally : p.second) {
                // Get everyone that ally hates
                auto lb = antipathy.lower_bound(ally), ub = antipathy.upper_bound(ally);
                for (; lb != ub and lb != antipathy.end(); ++lb)
                    hated.push_back(lb->second);
            }

            // Add antipathy between all alliance members and all hated
            for (int ally : p.second) {
                for (int hatee : hated)
                    antipathy.insert(make_pair(ally, hatee));
            }
        }
    }

    size_t get_num_alliances(UnionFind & uf) {
        unordered_set<int> parents;
        for (int i = 0; i < n_countries; ++i)
            parents.insert(uf.find(i));

        return parents.size();
    }

    string solve() {
        // Check size of alliance containing country with id 1 (index 0)
        return uf->size_of(0) > (n_countries / 2) ? "yes" : "no";
    }

    ~TestCase() {
        if (uf != nullptr) delete uf;
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
