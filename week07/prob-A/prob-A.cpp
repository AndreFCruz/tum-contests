#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

typedef unsigned int uint;

using namespace std;

class TestCase {
    uint street_len;
    uint n_lights;
    uint light_radius;

    vector<uint> light_pos;

public:
    explicit TestCase(istream& in) {
        in >> street_len >> n_lights >> light_radius;

        light_pos.resize(n_lights);
        for (uint i = 0; i < n_lights; ++i) {
            in >> light_pos[i];
        }
        sort(light_pos.begin(), light_pos.end());
    }

    string solve() {
        uint n_lights_on = 0;

        uint current_light = 0; // next light available for turning on
        uint current_pos = 0;   // next position to light up
        while (current_pos < street_len) {

            // Get farthest light up to current_pos + light_radius
            bool found = false;
            for (uint l = current_light; l < n_lights; ++l) {
                if (light_pos[l] <= current_pos + light_radius) {
                    found = true;
                    current_light = l;
                } else {
                    break;
                }
            }

            if (! found)
                return "impossible";

            current_pos = light_pos[current_light] + light_radius;
            n_lights_on += 1;
            current_light += 1;
        }

        return to_string(n_lights_on);
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
