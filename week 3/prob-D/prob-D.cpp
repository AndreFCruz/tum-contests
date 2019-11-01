// Week 3 - Problem D

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <exception>

using namespace std;

typedef unsigned int uint;

class TestCase {

public:
    explicit TestCase(istream& in) {

    }

    uint solve() {

    }
};

int main() {
    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}