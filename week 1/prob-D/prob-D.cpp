#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

typedef unsigned int uint;

#define NUM_GAMES 5

void handle_case() {
    int num_members;
    cin >> num_members;

    // Fetch team scores from stdin
    int tmp;
    auto scores = vector<vector<int>>(num_members, vector<int>(5));
    for (int i = 0; i < num_members; ++i) {
        for (int j = 0; j < NUM_GAMES; ++j) {
            cin >> tmp;
            scores.at(i).at(j) = tmp;
        }
    }

    // Sort scores of each team member
    for (uint i = 0; i < scores.size(); ++i)
        sort(scores.at(i).begin(), scores.at(i).end(), [](int x, int y) {return x > y;});

    // Sort team members
    sort(scores.begin(), scores.end(), [] (vector<int>& v1, vector<int>& v2) {
        for (uint i = 0; i < v1.size(); ++i) {
            if (v1.at(i) == v2.at(i)) continue;
            return v1.at(i) > v2.at(i);
        }
        return true;
    });

    // Output results to stdout
    for (uint i = 0; i < scores.size(); ++i) {
        for (uint j = 0; j < scores.at(i).size(); ++j) {
            cout << scores[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int num_cases;
    cin >> num_cases;

    for (int i = 0; i < num_cases; ++i) {
        cout << "Case #" << i+1 << ":\n";
        handle_case();
    }

    return EXIT_SUCCESS;
}
