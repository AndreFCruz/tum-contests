#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>

// Longest Common Subsequence Problem
// https://en.wikipedia.org/wiki/Longest_common_subsequence_problem
// https://stackoverflow.com/questions/32599640/circular-longest-increasing-subsequence

using namespace std;

class TestCase {
    string a, b;

public:
    explicit TestCase(istream& in) {
        in >> a >> b;
    }

    // Find longest common circular subsequence (with possible rotations)
    string solve() {

        // Reduce "Longest Common Subsequence" to "Longest Increasing Subsequence"
        // https://stackoverflow.com/questions/34656050/reducing-longest-common-subsequence-to-longest-increasing-subsequence

        // index characters from string a
        unordered_multimap<char, int> char_to_pos;
        for (int i = 0; i < a.size(); ++i)
            char_to_pos.insert(make_pair(a[i], i));

        vector<int> s3;
        for (int i = 0; i < b.size(); ++i) {
            auto it = char_to_pos.find(b[i]);
            if (it == char_to_pos.end()) continue;
            s3.push_back(it->second);
            char_to_pos.erase(it);
        }


        // Solve circular LIS
        // https://stackoverflow.com/questions/32599640/circular-longest-increasing-subsequence
        vector<int> sorted(s3);
        sort(sorted.begin(), sorted.end());

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
