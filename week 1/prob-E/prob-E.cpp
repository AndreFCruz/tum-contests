#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <functional>
#include <cmath>

#define DIGITS  "1234567890"
#define LETTERS "qwertyuiopasdfghjklzxcvbnm"

using namespace std;
typedef unsigned int uint;

enum State { NUMBER, OPERATION };

const map<string, function<int(int, int)>> operations = {
    {"plus", [](int x, int y) -> int {return x + y;}},
    {"minus", [](int x, int y) -> int {return x - y;}},
    {"times", [](int x, int y) -> int {return x * y;}},
    {"tothepowerof", [](int x, int y) -> int {return pow(x, y);}},
};

int evaluate_str(string s) {
    int num;
    string op;

    int value = 0;
    State state = NUMBER;
    size_t i = 0, new_i;
    while (i != s.npos) {
        // Extract number / operation
        if (state == NUMBER) {
            state = OPERATION;
            new_i = s.find_first_of(LETTERS, i);
            num = stoi(s.substr(i, new_i == s.npos ? s.npos : new_i - i));

            // Perform operation on given numbers
            if (op.size() == 0) { // start of expression
                value = num;
            } else {
                value = (operations.find(op)->second)(value, num);
            }
        } else if (state == OPERATION) {
            state = NUMBER;
            new_i = s.find_first_of(DIGITS, i);
            op = s.substr(i, new_i - i);
        }

        i = new_i;
    }

    return value;
}

int main() {
    int num_cases;
    cin >> num_cases;

    string s;
    for (int i = 0; i < num_cases; ++i) {
        cin >> s;
        cout << "Case #" << i+1 << ": " << evaluate_str(s) << endl;
    }

    return EXIT_SUCCESS;
}
