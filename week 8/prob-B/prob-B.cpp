#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>

using namespace std;

class TestCase {
    string a, b;

public:
    explicit TestCase(istream& in) {
        in >> a >> b;
    }

    static string reverse_string(string s) {
        return string(s.rbegin(), s.rend());
    }

    static int longest_common_subsequence(string a, string b) {
        vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1));

        // classical LCS
        for (size_t a_i = 1; a_i <= a.size(); ++a_i) {
            for (size_t b_i = 1; b_i <= b.size(); ++b_i) {
                if (a[a_i] == b[b_i])
                    dp[a_i][b_i] = dp[a_i - 1][b_i - 1] + 1;
                else
                    dp[a_i][b_i] = max(dp[a_i - 1][b_i], dp[a_i][b_i - 1]);
            }
        }
//        return dp.back().back();

//        cout << endl;
//        cout << endl;
//        for (auto row : dp) {
//            for (int val : row)
//                cout << val << " ";
//            cout << endl;
//        }
//        cout << endl;
//        cout << endl;

        // find largest difference of LCS for rotations of a and b
        int best = 0;
        for (size_t i = 0; i < a.size() / 2; ++i) {
            for (size_t j = 0; j < b.size() / 2; ++j) {
                best = max(best, dp[i + ((a.size()+1) / 2)][j + ((b.size()+1) / 2)] - dp[i][j]);
                // a and b will always be even-sized as they're each the concatenation of two equal strings
            }
        }

        return best;
    }

    // Find longest common circular subsequence (with possible rotations)
    int solve() {

        // All rotations are included in
        string rot_a = a + a.substr(0, a.size() - 1);
        string rot_b = b + b.substr(0, b.size() - 1);

        int a_b = longest_common_subsequence(rot_a, rot_b);
        int a_b_r = longest_common_subsequence(rot_a, reverse_string(rot_b));
        int a_r_b = longest_common_subsequence(reverse_string(rot_a), rot_b);
        int a_r_b_r = longest_common_subsequence(reverse_string(rot_a), reverse_string(rot_b));

        return max(max(a_b, a_b_r), max(a_r_b, a_r_b_r));
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
