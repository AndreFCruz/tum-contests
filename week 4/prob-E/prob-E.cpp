// Week 4 - Problem E

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class TestCase {
    uint n_vertices;
    multimap<uint, uint> edges;
    vector<uint> costs;

public:
    explicit TestCase(istream& in) {

    }

    string solve() {

        return "impossibruuuu";
    }
};

int main() {
    ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
