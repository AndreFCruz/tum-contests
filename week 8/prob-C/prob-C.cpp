#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

// Stacking Boxes
// http://www.geeksforgeeks.org/dynamic-programming-set-21-box-stacking-problem/

using namespace std;

class TestCase {
public:
    explicit TestCase(istream& in) {

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
