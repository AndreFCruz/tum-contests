#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>

using namespace std;

constexpr int MAX_INT = numeric_limits<int>::max();

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

    static vector<int> min_num_coins_bottom_up(int money, const vector<int>& coins) {
        vector<int> used_coin(money+1);
        vector<int> dp(money+1, MAX_INT);

        dp[0] = 0; // base case: 0 coins for value 0
        for (int i = 1; i <= money; ++i) {
            for (int j = 0; j < (int) coins.size(); ++j) {
                if (coins[j] > i) break;
                int sub_res = dp[i-coins[j]];
                if (sub_res != MAX_INT && sub_res + 1 < dp[i]) {
                    dp[i] = sub_res + 1;
                    used_coin[i] = j;
                }
            }
        }

        // Reconstruct answer
        vector<int> coins_in_answer(coins.size(), 0);
        int v = money;
        while (v > 0) {
            coins_in_answer[used_coin[v]] += 1;
            v -= coins[used_coin[v]];
        }
        return coins_in_answer;
    }

    static int min_num_coins(int money, const vector<int>& coins, unordered_map<int, int>& cache) {
        if (money < coins[1]) return money; // _money_ number of coins of value 1

        int best = money;
        for (int c_idx = coins.size() - 1; c_idx > 0; --c_idx) { // which coin to use
            if (coins[c_idx] > money) continue;

            for (int n = money / coins[c_idx]; n > 0; --n) { // how many coins to use
                int money_left = money - (coins[c_idx] * n);
                if (cache.count(money_left) == 0) {
                    cache[money_left] = min_num_coins(money_left, coins, cache);
                }

                int ans = cache.at(money_left) + n;
                if (best > ans)
                    best = ans;
            }
        }

        return best;
    }

    void solve() {
//        unordered_map<int, int> cache;
//        int ans = min_num_coins(money, n_coins - 1, coins, cache);

        vector<int> used_coins = min_num_coins_bottom_up(money, coins);
        for (int c : used_coins)
            cout << c << " ";
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": ";
        TestCase(cin).solve();
        cout << endl;
    }

    return EXIT_SUCCESS;
}
