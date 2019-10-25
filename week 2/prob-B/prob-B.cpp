#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <iomanip>

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

    static double sigmoid(double x) {
        return 1 / (1 + exp(-x));
    }

    /**
     * Optimize p_guess through gradient descent of the error function
     * Error = payoff_with_current_guess - cost_to_draw
     * @return estimate of p_guess
     */
    double gradient_descent(double p_start=0.5, double gamma=0.1) {
        double current_p, next_p = p_start;
        double loss, payoff;

        do {
            current_p = next_p;
            payoff = payoff_for_p(current_p);
//            loss = cross_entropy_loss(payoff, cost_to_draw);
            loss = 2 * (sigmoid(payoff - cost_to_draw) - 0.5);
            next_p = current_p - gamma * loss;

            // decay learning_rate
            gamma = gamma < 0.01 ? 0.01 : gamma * 0.9;
        } while (abs(next_p - current_p) > MAX_ERROR * 10e-4l);

        return next_p;
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
        cout << "Case #" << i + 1 << ": " << setprecision(10) << TestCase(cin).gradient_descent() << endl;

    return EXIT_SUCCESS;
}
