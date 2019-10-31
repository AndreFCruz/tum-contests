#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <string>

using namespace std;

class TestCase {
public:
    TestCase(istream& in) {

    }
};

int main() {
    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}