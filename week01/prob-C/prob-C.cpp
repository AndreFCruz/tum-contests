#include <iostream>
#include <string>
#include <algorithm>

#define C 299792458ULL

using namespace std;

typedef unsigned long long ullong;

ullong calc_energy(unsigned mass) {
    return (ullong) mass * (C * C);
}

int main() {
    int num_cases;
    cin >> num_cases;

    unsigned mass;
    for (int i = 0; i < num_cases; ++i) {
        cin >> mass;
        cout << "Case #" << i+1 << ": " << calc_energy(mass) << endl;
    }

    return EXIT_SUCCESS;
}
