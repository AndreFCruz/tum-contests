#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <cmath>

#define MAX_ERROR   10e-6l

using namespace std;


class TestCase {
public:
    int num_prizes;
    int cost_to_draw;
    vector<int> prizes;

    TestCase(istream& in) {
        in >> num_prizes >> cost_to_draw;

        int tmp;
        for (int i = 0; i < num_prizes; ++i) {
            in >> tmp;
            prizes.push_back(tmp);
        }
    }

    string solve() {
        double p_guess = 0.5;
        double learning_rate = 0.5;

        double payoff, error;
        do {
            payoff = payoff_for_p(p_guess);
            error = cost_to_draw - payoff;
            p_guess += error * learning_rate;
//            learning_rate = learning_rate < 0.01 ? learning_rate : 0.9 * learning_rate; // decay learning-rate
        } while (abs(payoff) > MAX_ERROR);

        return to_string(p_guess);
    }

    double payoff_for_p(double p_guess) {
        double payoff = 0;
        double curr_p = p_guess;
        for (size_t i = 0; i < prizes.size(); ++i) {
            payoff += prizes[i] * curr_p;
            curr_p *= p_guess;
        }
        return payoff;
    }
};

int main() {
    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i)
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;

    return EXIT_SUCCESS;
}
