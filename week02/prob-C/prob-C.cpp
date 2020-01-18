#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iomanip>

using namespace std;

#define MAX_ERROR   1e-5l

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
    bool is_valid(double min_pole_dist) {
        // How many posts fit before canyon ?
        int before = 1 + (canyon_start / min_pole_dist);

        // Next post position after canyon
        double next_pos = max((double) canyon_end, min_pole_dist * before);
        if (next_pos > length) return before >= num_posts;

        // How many posts fit after canyon ?
        int after = 1 + ((length - next_pos) / min_pole_dist);
        return before + after >= num_posts;
    }

    /**
     * Same result as is_valid(double), but with slightly different algorithm.
     */
    bool is_valid_old(double min_pole_dist) {
        double dist = 0;
        int poles = 1;  // a pole always goes at position x=0
        bool after_canyon = false;
        while (dist <= length and poles < num_posts) {
            double new_dist;
            if (dist + min_pole_dist <= canyon_start) {
                new_dist = dist + min_pole_dist;
            } else if (! after_canyon) {
                new_dist = max((double) canyon_end, dist + min_pole_dist);
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

    double solve() {
        double low = 0, high = length / (num_posts - 1.), mid;
        while (high - low > MAX_ERROR) {
            mid = (high + low) / 2;
            if (is_valid(mid))
                low = mid;
            else
                high = mid;
        }

        return high;
//        return (high + low) / 2;
    }
};

int main() {
    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i)
        cout << "Case #" << i + 1 << ": " << setprecision(10)
             << TestCase(cin).solve() << endl;

    return EXIT_SUCCESS;
}
