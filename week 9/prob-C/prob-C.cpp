#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>
#include <cmath>

typedef unsigned long ulong;

using namespace std;

// Greatest common divisor
ulong gcd(ulong a,ulong b) {
    ulong temp;
    while(b > 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/**
 * Extended Euclidean Algorithm.
 * Outputs gcd(a, b); and integers (x, y) with gcd(a, b) = ax + by
 * @param a
 * @param b
 * @param x
 * @param y
 * @return gcd(a, b)
 */
//ulong extended_gcd(ulong a, ulong b, ulong& x, ulong& y) {
//    ulong s = 0, s_prime = 1;
//    ulong t = 1, t_prime = 0;
//    ulong r = b, r_prime = a;
//
//    ulong tmp;
//    while (r != 0) {
//        ulong q = r_prime / r;
//
//        // (r', r) <- (r, r' - q*r)
//        tmp = r_prime;
//        r_prime = r;
//        r = tmp - q * r;
//
//        // (s', s) <- (s, s' - q*s)
//        tmp = s_prime;
//        s_prime = s;
//        s = tmp - q * s;
//
//        // (t', t) <- (t, t' - q*t)
//        tmp = t_prime;
//        t_prime = t;
//        t = tmp - q * t;
//    }
//
//    x = s_prime;
//    y = t_prime;
//    return r_prime;
//}

// Least common multiple
ulong lcm(ulong a, ulong b) {
    return (a * b) / gcd(a, b);
}


class TestCase {
    ulong n_friends;
    vector<ulong> candy_eaten;

public:
    explicit TestCase(istream& in) {
        in >> n_friends;
        candy_eaten.reserve(n_friends);
        for (size_t i = 0; i < n_friends; ++i)
            in >> candy_eaten[i];
    }
    
    string solve() {
        // Possibilities for candy eating
        ulong max = pow(2, this->n_friends);
        vector<ulong> candy_eating_combinations;
        for (ulong j = 0; j < max; ++j) {
            ulong sum = 1;
            for(ulong i = 0; i < n_friends; ++i) {
                // Check if i-th LSB is on
                if (1 == ( (j >> i) & 1)) {
                    sum = sum + candy_eaten[i];
                }
            }
            candy_eating_combinations.push_back(sum);
        }

        ulong x = lcm(candy_eating_combinations[0], candy_eating_combinations[1]);
        for (ulong i = 2; i < candy_eating_combinations.size(); ++i) {
            x = lcm(x, candy_eating_combinations[i]);
        }

        return to_string(x);
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
