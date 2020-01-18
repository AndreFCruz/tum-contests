#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

struct Tournament {
    int64_t start;
    int64_t end;
    int64_t prize;
};

using namespace std;

class TestCase {
    int64_t n_tournaments;
    map<int64_t, vector<Tournament>> tournaments;

public:
    explicit TestCase(istream& in) {
        in >> n_tournaments;

        for (int64_t i=0; i < n_tournaments; ++i) {
            int64_t a, b, p;
            in >> a; // start
            in >> b; // end
            in >> p; // prize money
            tournaments[b].push_back({a, b, p});
        }
    }

    static int64_t solve_aux_dp(int64_t t, map<int64_t, vector<Tournament>> &tournaments, vector<int64_t> &cache) {
        if (cache[t] > 0) return cache[t]; // value already cached
        if (t == 0) return 0;   // base case; t=0, there are no tournaments to play

        // current max is the maximum obtained at the previous time step
        int64_t currentMax = solve_aux_dp(t - 1, tournaments, cache);
        cache[t] = currentMax;

        for (auto tournament : tournaments[t]) {
            // is playing this tournament better than the current max?
            currentMax = max(currentMax, tournament.prize + solve_aux_dp(tournament.start - 1, tournaments, cache));
            cache[t] = currentMax;
        }

        return currentMax;
    }

    string solve() {
        int64_t maxTime = tournaments.rbegin()->first;
        vector<int64_t> cache(maxTime + 1, 0);

        int64_t ans = solve_aux_dp(maxTime, tournaments, cache);
        return to_string(ans);
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
