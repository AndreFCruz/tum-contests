#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;

#define MAX_ERROR   10e-4

class TestCase {
public:
    int length;
    int num_posts;
    int canyon_start, canyon_end;

    TestCase(istream & in) {
        in >> length >> num_posts >> canyon_start >> canyon_end;
    }

    /**
     * Check whether this configuration of pole positions is valid
     * (characterized by its minimum distance between poles).
     * @param min_pole_dist minimum distance between poles in this configuration.
     * @return whether this configuration is valid.
     */
    bool is_valid(float min_pole_dist) {
        float dist = 0;
        int poles = 1;  // a pole always goes at position x=0
        bool after_canyon = false;
        while (dist <= length and poles < num_posts) {
            float new_dist;
            if (dist + min_pole_dist <= canyon_start) {
                new_dist = dist + min_pole_dist;
            } else if (! after_canyon) {
                new_dist = max((float) canyon_end, dist + min_pole_dist);
                after_canyon = true;
            } else if (dist + min_pole_dist <= length) {
                new_dist = dist + min_pole_dist;
                assert(dist >= canyon_end);
            } else
                break;

            poles += 1;
            dist = new_dist;
        }

        return poles >= num_posts;
    }

    float solve() {
        float low = 0, high = length / (num_posts - 1.), mid;
        while (high - low > MAX_ERROR * 10e-2) {
            mid = (high + low) / 2;
            if (is_valid(mid))
                low = mid;
            else
                high = mid;
        }

        return (high + low) / 2;
    }
};

int main() {
    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i)
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;

    return EXIT_SUCCESS;
}
