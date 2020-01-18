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

        vector<bool> visited(num_rooms + 1, false);
        set<uint> nodes;
        nodes.insert(start_node);
        visited[start_node] = true;
        uint curr_water_lv = init_water_lv, last_used_water_lv = init_water_lv;

        // While there are reachable nodes
        while (! nodes.empty()) {
            uint n = *nodes.begin();
            nodes.erase(nodes.begin());

            // If it's a control room, lower water level
            auto it = control_rooms.find(n);
            if (it != control_rooms.end()) {
                curr_water_lv = min(curr_water_lv, it->second);
            }

            // Access all possible hallways
            auto hallway_it = hallways.begin();
            while (hallway_it != hallways.end() and hallway_it->water_level >= curr_water_lv) {
                bool used = false;
                if (visited[hallway_it->a] and !visited[hallway_it->b]) {
                    nodes.insert(hallway_it->b);
                    visited[hallway_it->b] = true;
                    used = true;
                }
                else if (!visited[hallway_it->a] and visited[hallway_it->b]) {
                    nodes.insert(hallway_it->a);
                    visited[hallway_it->a] = true;
                    used = true;
                }
                else if (visited[hallway_it->a] and visited[hallway_it->b]) {
                    hallway_it = hallways.erase(hallway_it);
                    continue;
                }

                // If this hallway was used
                if (used) {
//                    cout << "Using hallway <" << hallway_it->a << "; " << hallway_it->b << "> at " << hallway_it->water_level << endl; // TODO delete
                    last_used_water_lv = min(last_used_water_lv, hallway_it->water_level);
                    hallway_it = hallways.erase(hallway_it);
                } else {
                    ++hallway_it;
                }
            }
        }

        // Check if all nodes were visited, and return appropriately
        for (uint i = 1; i < visited.size(); ++i)
            if (! visited[i]) return "impossible";

        return to_string(last_used_water_lv);
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
