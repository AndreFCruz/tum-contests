#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <sstream>
#include <iomanip>

using namespace std;

typedef uint32_t clck_t;

enum digit : uint8_t {
    // Each bit indicates a segment of the clock
    n0 = (1 << 0) + (1 << 1) + (1 << 2) + (0 << 3) + (1 << 4) + (1 << 5) + (1 << 6),
    n1 = (0 << 0) + (0 << 1) + (1 << 2) + (0 << 3) + (0 << 4) + (1 << 5) + (0 << 6),
    n2 = (1 << 0) + (0 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (0 << 5) + (1 << 6),
    n3 = (1 << 0) + (0 << 1) + (1 << 2) + (1 << 3) + (0 << 4) + (1 << 5) + (1 << 6),
    n4 = (0 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (0 << 4) + (1 << 5) + (0 << 6),
    n5 = (1 << 0) + (1 << 1) + (0 << 2) + (1 << 3) + (0 << 4) + (1 << 5) + (1 << 6),
    n6 = (1 << 0) + (1 << 1) + (0 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6),
    n7 = (1 << 0) + (0 << 1) + (1 << 2) + (0 << 3) + (0 << 4) + (1 << 5) + (0 << 6),
    n8 = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6),
    n9 = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (0 << 4) + (1 << 5) + (1 << 6),
};

class TestCase {
    const unordered_map<char, uint8_t> charToDigit = {
            {'0', digit::n0},
            {'1', digit::n1},
            {'2', digit::n2},
            {'3', digit::n3},
            {'4', digit::n4},
            {'5', digit::n5},
            {'6', digit::n6},
            {'7', digit::n7},
            {'8', digit::n8},
            {'9', digit::n9},
    };

    const unordered_map<uint8_t, char> digitToChar = {
            {digit::n0, '0'},
            {digit::n1, '1'},
            {digit::n2, '2'},
            {digit::n3, '3'},
            {digit::n4, '4'},
            {digit::n5, '5'},
            {digit::n6, '6'},
            {digit::n7, '7'},
            {digit::n8, '8'},
            {digit::n9, '9'},
    };

    const unordered_map<uint8_t, char> digitToValue = {
            {digit::n0, 0},
            {digit::n1, 1},
            {digit::n2, 2},
            {digit::n3, 3},
            {digit::n4, 4},
            {digit::n5, 5},
            {digit::n6, 6},
            {digit::n7, 7},
            {digit::n8, 8},
            {digit::n9, 9},
    };

    // Sequential clock observations
    vector<clck_t> clock_observations;

    // Keeps candidate solutions, to be filtered later regarding whether they fit all clock observations
    unordered_map<clck_t, int> candidate_sols;

public:
    explicit TestCase(istream& in) {
        int n;
        in >> n;

        for (int i = 0; i < n; ++i) {
            uint8_t ht, hu, mt, mu;
            in >> ht >> hu;
            in.ignore(); // ignore ':' character
            in >> mt >> mu;

            ht = charToDigit.at(ht);
            hu = charToDigit.at(hu);
            mt = charToDigit.at(mt);
            mu = charToDigit.at(mu);

            clock_observations.push_back( ((clck_t) mu) + ((clck_t) mt << 7u) + ((clck_t) hu << 14u) + ((clck_t) ht << 21u) );
        }
        in.ignore(); // ignore last new-line
    }

    string clockToString(clck_t c) {
        uint8_t ht = (c << (32u - 28u)) >> 25u; // hours tens
        uint8_t hu = (c << (32u - 21u)) >> 25u; // hours units
        uint8_t mt = (c << (32u - 14u)) >> 25u; // minutes tens
        uint8_t mu = (c << (32u - 7u)) >> 25u;  // minutes units

        stringstream ss;
        ss << digitToChar.at(ht) << digitToChar.at(hu) << ':' << digitToChar.at(mt) << digitToChar.at(mu);
        return ss.str();
    }

    int clockToMinutes(clck_t c) {
        uint8_t ht = (c << (32u - 28u)) >> 25u;
        uint8_t hu = (c << (32u - 21u)) >> 25u;
        uint8_t mt = (c << (32u - 14u)) >> 25u;
        uint8_t mu = (c << (32u - 7u)) >> 25u;

        return digitToValue.at(ht) * 600 + digitToValue.at(hu) * 60 + digitToValue.at(mt) * 10 + digitToValue.at(mu);
    }

    static bool isValidDigit(uint8_t x) {
        return !(x != n0 && x != n1 && x != n2 && x != n3 && x != n4 && x != n5 && x != n6 && x != n7 && x != n8 &&
                 x != n9);
    }

    static void printTimeInMins(int n_min) {
        int hours = n_min / 60;
        int minutes = n_min % 60;
        cout << setfill('0') << setw(2) << hours << ":" << setfill('0') << setw(2) << minutes;
    }

    bool test(clck_t c) {
        uint8_t hd = (c << (32u - 28u)) >> 25u;
        uint8_t hu = (c << (32u - 21u)) >> 25u;
        uint8_t md = (c << (32u - 14u)) >> 25u;
        uint8_t mu = (c << (32u - 7u)) >> 25u;

        if ( hd != n0 && hd != n1 && hd != n2)
            return false;
        else if ( hd == n2 && ( hu != n0 && hu != n1 && hu != n2 && hu != n3))
            return false;
        else if (!isValidDigit(hu))
            return false;
        else if ( md != n0 && md != n1 && md != n2 && md != n3 && md != n4 && md != n5)
            return false;

        return isValidDigit(mu);
    }

    bool generateAndTest(clck_t c, int bit_idx, clck_t mask, int min_offset) {
        // Base case -- no more changes can be made to the clock
        if (bit_idx == -1) {
            bool valid = test(c);
            if (valid) {
                int t_mins = clockToMinutes(c) - min_offset;
                if (t_mins < 0)
                    t_mins += 24 * 60;
                candidate_sols[t_mins] += 1;
            }
            return valid;
        }

        if ((c >> bit_idx) & 1) // this bit is set
            return generateAndTest(c, bit_idx - 1, mask, min_offset);

        if ((mask >> bit_idx) & 1)
            return generateAndTest(c, bit_idx - 1, mask, min_offset);

        bool flag = false;
        c = c | (1 << bit_idx);
        flag = generateAndTest(c, bit_idx - 1, mask, min_offset) || flag;

        c = c & ~(1 << bit_idx);
        flag = generateAndTest(c, bit_idx - 1, mask, min_offset) || flag;

        return flag;
    }

    void solve() {
        // Mask: 1 if the corresponding clock segment was ever turned on (guaranteed to be working)
        clck_t mask = 0;
        for (clck_t c : this->clock_observations)
            mask |= c;

        for (size_t i = 0; i < clock_observations.size(); ++i)
            generateAndTest(clock_observations[i], 27, mask, i);

        vector<int> valid_times;
        for (const auto & p : this->candidate_sols) {
            if (p.second == (int) clock_observations.size())
                valid_times.push_back(p.first);
        }

        // Sort times through time (not lexicographically)
        sort(valid_times.begin(), valid_times.end());

        // If no valid solutions are found
        if (valid_times.empty()) {
            cout << "none" << endl;
            return;
        }

        // Else, print valid solutions
        for (int t : valid_times) {
            printTimeInMins(t);
            cout << endl;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ":" << endl;
        TestCase(cin).solve();
    }

    return EXIT_SUCCESS;
}
