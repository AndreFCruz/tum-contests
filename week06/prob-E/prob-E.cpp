#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

#define FIRST   true
#define SECOND  false

class TestCase {
    int bakers, judges;
    vector<vector<int>> prefs;

public:
    explicit TestCase(istream& in) {
        in >> bakers >> judges;
        prefs.resize(judges, vector<int>());

        for (int i = 0; i < judges; ++i) {
            int n;
            while (true) {
                in >> n;
                if (n == 0) break;
                prefs[i].push_back(n);
            }
        }
    }

    bool test(vector<bool>& recipes) {
//        cout << "Testing ";
//        for (bool b : recipes)
//            cout << (b ? "0 " : "1 ");
//        cout << endl;

        for (int j = 0; j < judges; ++j) {
            bool sat = false;

            for (int p : prefs[j]) {
                if (recipes[p > 0 ? p-1 : (-p) - 1] == (p > 0 ? FIRST : SECOND)) {
                    sat = true;
                    break;
                }
            }
            if (!sat) return false;
        }

        return true;
    }

    bool generate_and_test(vector<bool>& recipes, size_t idx) {
        // Generate combinations of recipe assignments to each baker (either FIRST or SECOND)
        // Test whether recipe assignment satisfies bakers

        // Base case
        if (idx == recipes.size()) return test(recipes);

        // Recursive case
        recipes[idx] = true;
        if (generate_and_test(recipes, idx + 1)) return true;

        recipes[idx] = false;
        if (generate_and_test(recipes, idx + 1)) return true;

        return false;
    }

    string solve() {
        // SAT Problem, generate recipe clauses and test for judge satisfiability
        // O(n * 2^m)
        vector<bool> recipes(bakers, FIRST);
        return generate_and_test(recipes, 0) ? "yes" : "no";
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
