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

struct Case {
    int x, y, z;
    int used = 0;

    Case() = default;
    Case(int x, int y, int z) : x(x), y(y), z(z) {}

    /**
     * Position 0: (x,y,z)
     * Position 1: (y,z,x)
     * Position 2: (z,x,y)
     * @param pos position of the box
     * @return the horizontally-projected dimensions of the box
     */
//    tuple<int, int, int> get_dims(int pos = 0) {
//        switch (pos) {
//            case 0:
//                return make_tuple(x, y, z);
//            case 1:
//                return make_tuple(y, z, x);
//            case 2:
//                return make_tuple(z, x, y);
//            default:
//                throw invalid_argument("pos in {0,1,2}");
//        }
//    }
};

class TestCase {
    int n_case_types;
    int height;
    vector<Case> cases;

public:
    explicit TestCase(istream& in) {
        in >> height >> n_case_types;

        cases.resize(n_case_types * 3);
        for (int i = 0; i < n_case_types * 3; i += 3) {
            // 1st rotation: (x,y,z)
            in >> cases[i].x >> cases[i].y >> cases[i].z;

            // 2nd rotation: (y,z,x)
            cases[i+1].x = cases[i].y;
            cases[i+1].y = cases[i].z;
            cases[i+1].z = cases[i].x;

            // 3rd rotation: (z,x,y)
            cases[i+1].x = cases[i].z;
            cases[i+1].y = cases[i].x;
            cases[i+1].z = cases[i].y;
        }

        // Sort by decreasing base area
        // A box cannot be stacked on top of another with larger base area (the opposite may not be true)
        sort(cases.begin(), cases.end(), [](const Case& c1, const Case& c2) {
            return c1.x * c1.y > c2.x * c2.y;
        });
    }

    /**
     * Finds the maximum height of a stack ending in box c_idx
     * @param c_idx the index of the current case to use
     * @param cases the cases available to use
     * @param cache cache[i] saves the max stack height for which case i is at the top
     * @return
     */
    static int max_height(int c_idx, const vector<Case>& cases, vector<int>& cache) {
        if (cache[c_idx] != -1) return cache[c_idx]; // already computed result

        // For all boxes up to this one
        int max_stack_height = cases[c_idx].z;
        for (int j = 0; j < c_idx; ++j) {
            // Stack case c_idx on top of structure ending with case j ?
            if (cases[j].x > cases[c_idx].x and cases[j].y > cases[c_idx].y) {
                max_stack_height = max(max_stack_height, max_height(j, cases, cache) + cases[c_idx].z);
            }
        }

        cache[c_idx] = max_stack_height;
        return cache[c_idx];
    }

    string solve() {
        vector<int> cache(cases.size(), -1);
        max_height(cases.size() - 1, cases, cache);

        for (int msh : cache) {
            if (msh >= this->height) return "yes";
        }
        return "no";
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
