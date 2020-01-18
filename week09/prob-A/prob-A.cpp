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

// Greatest common divisor between a list of numbers

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

    static uint gcd(uint a,uint b) {
        uint temp;
        while(b > 0) {
            temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    static uint gcd(const vector<uint>& nums) {
        uint result = nums[0];
        for (size_t i = 1; i < nums.size(); ++i)
            result = gcd(result, nums[i]);

        return result;
    }

    string solve() {
        return to_string(gcd(nums));
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
