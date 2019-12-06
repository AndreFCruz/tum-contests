#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

// Longest Common Subsequence Problem
// https://en.wikipedia.org/wiki/Longest_common_subsequence_problem

using namespace std;


class TestCase {
public:
    explicit TestCase(istream& in) {

    }

    string solve() {
        return "...";
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
