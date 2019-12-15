#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <map>
#include <limits>
#include <queue>

typedef unsigned int uint;

using namespace std;

// Largest common divisor between a list of numbers

class TestCase {
    vector<uint> nums;

public:
    explicit TestCase(istream& in) {
        int n_nums;
        in >> n_nums;

        nums.resize(n_nums);
        for (int i = 0; i < n_nums; ++i)
            in >> nums[i];
    }

    static uint lgd(vector<uint>& nums) {
        uint min_num = *min(nums.begin(), nums.end());

        uint max_div = 1;
        for (uint i = 1; i <= min_num; ++i) {
            bool all_dividable = true;
            for (uint n : nums) {
                if (n % i != 0) {
                    all_dividable = false;
                    break;
                }
            }

            if (all_dividable)
                max_div = i;
        }

        return max_div;
    }

    string solve() {
        return to_string(lgd(nums));
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
