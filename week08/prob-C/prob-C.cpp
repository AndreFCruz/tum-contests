#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

// Stacking Boxes
// http://www.geeksforgeeks.org/dynamic-programming-set-21-box-stacking-problem/

using namespace std;

struct Case {
    int x, y, z;

    Case() = default;
    Case(int x, int y, int z) : x(x), y(y), z(z) {}
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
            Case aux;
            in >> aux.x >> aux.y >> aux.z;
            // NOTE: for simplicity always keep x > y
            // if 2 boxes stack can stack, then aligning x with their largest length will keep this property

            // 1st rotation: (x,y,z)
            cases[i].x = max(aux.x, aux.y);
            cases[i].y = min(aux.x, aux.y);
            cases[i].z = aux.z;

            // 2nd rotation: (y,z,x)
            cases[i+1].x = max(aux.y, aux.z);
            cases[i+1].y = min(aux.y, aux.z);
            cases[i+1].z = aux.x;

            // 3rd rotation: (z,x,y)
            cases[i+2].x = max(aux.z, aux.x);
            cases[i+2].y = min(aux.z, aux.x);
            cases[i+2].z = aux.y;
        }

        // Sort by decreasing base area
        // A box cannot be stacked on top of another with larger base area (the opposite may not be true)
        sort(cases.begin(), cases.end(), [](const Case& c1, const Case& c2) {
            return c1.x * c1.y > c2.x * c2.y;
        });
    }

    static int max_height_bottom_up(const vector<Case>& cases) {

        // Keeps current maximum stack height for stacks ending in case i
        vector<int> msh(cases.size());
        for (size_t i = 0; i < msh.size(); ++i)
            msh[i] = cases[i].z;

        // Compute optimized msh values in bottom up manner
        for (size_t i = 1; i < cases.size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (cases[i].x < cases[j].x and cases[i].y < cases[j].y and msh[i] < msh[j] + cases[i].z)
                    msh[i] = msh[j] + cases[i].z;
            }
        }


        /* Pick maximum of all msh values */
        int best = -1;
        for (size_t i = 0; i < cases.size(); ++i)
            if (best < msh[i]) best = msh[i];
        return best;
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
//        vector<int> cache(cases.size(), -1);
//        max_height(cases.size() - 1, cases, cache);
//
//        for (int msh : cache) {
//            if (msh >= this->height) return "yes";
//        }
//        return "no";

        return max_height_bottom_up(this->cases) >= this->height ? "yes" : "no";
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
