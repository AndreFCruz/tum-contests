#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>

using namespace std;


class TestCase {
    int money;
    int n_coins;
    vector<int> coins;

public:
    explicit TestCase(istream& in) {
        in >> n_coins >> money;

        coins.resize(n_coins);
        for (int i = 0; i < n_coins; ++i)
            in >> coins[i];
    }

    static int min_num_coins(int money, int coin_idx_hint, const vector<int>& coins, unordered_map<int, int>& cache) {
        if (money < coins[1]) return money; // _money_ number of coins of value 1

        int best = money;
        for (int c_idx = coin_idx_hint; c_idx >= 0; --c_idx) {
            if (coins[c_idx] > money) continue;

            int money_left = money % coins[c_idx];
            if (cache.count(money_left) == 0) {
                cache[money_left] = min_num_coins(money_left, c_idx - 1, coins, cache);
            }

            int ans = cache.at(money_left) + (money / coins[c_idx]);
            if (best > ans)
                best = ans;
        }

        return best;
    }

    string solve() {
        unordered_map<int, int> cache;
        int ans = min_num_coins(money, n_coins - 1, coins, cache);
        return to_string(ans);
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
