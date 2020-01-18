#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <iomanip>

#define MAX_ERROR   1e-6l

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

     * @return estimate of p
     */
    /**
     * Optimize p_guess through gradient descent of the error function
     * Error = payoff_with_current_guess - cost_to_draw
     * @param p_start   initial guess for p
     * @param gamma     learning rate
     * @param min_gamma minimum learning rate after decay
     * @param decay     learning rate decay
     * @return          estimate of p
     */
    double gradient_descent(double p_start=0.5, double gamma=0.1, double min_gamma=0.01, double decay=0.9) {
        double current_p, next_p = p_start;
        double loss, payoff;

        do {
            current_p = next_p;
            payoff = payoff_for_p(current_p);
            loss = 2 * (sigmoid(payoff - cost_to_draw) - 0.5);  // Use sigmoid for squishing loss, then remap it to [-1, 1]
            next_p = current_p - gamma * loss;

            // decay learning_rate
            gamma = gamma < min_gamma ? min_gamma : gamma * decay;
        } while (abs(next_p - current_p) > MAX_ERROR * 1e-2l);

        return next_p;
    }

    /**
     * Binary search among possible p values.
     * @return estimate of p
     */
    double binary_search() {
        double left = 0, right = 1, mid;
        double payoff;

        do {
            mid = (right + left) / 2;
            payoff = payoff_for_p(mid);
            if (payoff > cost_to_draw)
                right = mid;
            else if (payoff < cost_to_draw)
                left = mid;
            else return mid;
        } while(right - left > MAX_ERROR);

        return (left + right) / 2;
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
        cout << "Case #" << i + 1 << ": " << setprecision(10) << TestCase(cin).binary_search() << endl;

    return EXIT_SUCCESS;
}
