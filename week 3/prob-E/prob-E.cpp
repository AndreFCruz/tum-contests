// Week 3 - Problem E

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <set>
#include <limits>

using namespace std;

typedef unsigned int uint;

class UnionFind {
private:
    size_t n;
    vector<uint> parent;
    vector<uint> size;

public:
    UnionFind(size_t n) : n(n) {
        this->size = vector<uint>(n, 1);
        this->parent = vector<uint>(n);

        uint i = 0;
        generate(parent.begin(), parent.end(), [&]() {
            return i++;
        });

    }

    uint find(uint a) {
        // Find root
        uint root = a;
        while (parent[root] != root)
            root = parent[root];

        // Compress path
        uint current = a, next_elem;
        while (current != root) {
            next_elem = parent[current];
            parent[current] = root;
            current = next_elem;
        }

        return root;
    }

    uint merge(uint a, uint b) {
        a = find(a);
        b = find(b);
        if (a == b) // a and b are already merged
            return a;

        // Weighted union: update smaller component
        uint a_size = size[a], b_size = size[b];
        if (a_size < b_size)
            swap(a, b);

        parent[b] = a;
        // Update size accordingly
        size[a] = a_size + b_size;
        return a;
    }

    uint size_of(uint a) {
        return size[find(a)];
    }
};

struct Hallway {
    uint a, b;          // the hallway's connecting rooms
    uint water_level;   // to use this hallway, the water must be at this level
};

struct HallwayCmp {
    bool operator()(const Hallway& h1, const Hallway& h2) const {
        if (h1.water_level != h2.water_level)
            return h1.water_level > h2.water_level;
        return h1.a != h2.a ? h1.a < h2.a : h1.b < h2.b;
    }
};

class TestCase {
    uint num_rooms;
    uint init_water_lv;
    set<Hallway, HallwayCmp> hallways;
    map<uint, uint> control_rooms;      // map[room] = water_level_drain

public:
    explicit TestCase(istream& in) {
        uint num_hallways, num_controls;
        in >> this->num_rooms >> num_hallways >> num_controls >> this->init_water_lv;

        for (uint i = 0; i < num_hallways; ++i) {
            Hallway h;
            in >> h.a >> h.b >> h.water_level;
            hallways.insert(h);
        }

        uint n1, n2;
        for (uint i = 0; i < num_controls; ++i) {
            in >> n1 >> n2;
            control_rooms.insert(make_pair(n1, n2));
        }
    }

    string solve(uint start_node = 1) {

        UnionFind uf(num_rooms + 1);
        uint curr_water_lv = init_water_lv;

        // Unite all accessible nodes in subsets
        auto h_it = hallways.begin();
        for ( ; h_it != hallways.end() and h_it->water_level >= curr_water_lv; ++h_it)
            uf.merge(h_it->a, h_it->b);

        // While there are inaccessible rooms
        while (uf.size_of(uf.find(start_node)) < num_rooms) {
            uint objective_water_lv = h_it->water_level;
            vector<uint> accessible_rooms;
            for (uint n = 1; n <= num_rooms; ++n) {
                if (uf.find(start_node) == uf.find(n))
                    accessible_rooms.push_back(n);
            }

            bool progress = false;
            for (uint room : accessible_rooms) {
                auto r_it = control_rooms.find(room);
                if (r_it != control_rooms.end() and r_it->second <= objective_water_lv) {
                    curr_water_lv = objective_water_lv;
                    progress = true;
                }
            }

            if (! progress)
                return "impossible";

            // Unite all accessible nodes
            for (; h_it != hallways.end() and h_it->water_level >= curr_water_lv; ++h_it)
                uf.merge(h_it->a, h_it->b);
        }

        return to_string(curr_water_lv);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
