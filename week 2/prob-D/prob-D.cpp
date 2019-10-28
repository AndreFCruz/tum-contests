#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

using namespace std;

typedef uint32_t uint;

struct Station {
    uint id;     // Station's number
    uint u;      // Station's room range start
    uint v;      // Station's room range end

    Station(uint id, uint u, uint v) : id(id), u(u), v(v) {}
};

struct StationMinCmp {
    bool operator()(const Station& s1, const Station& s2) const {
        if (s1.u != s2.u) return s1.u < s2.u;
        else return s1.id < s2.id;
    }
};

struct StationIdCmp {
    bool operator()(const Station& s1, const Station& s2) const { return s1.id < s2.id; };
};

class TestCase {
    set<Station, StationMinCmp> stations;   // Ordered set of Stations
    vector<uint> friends_lines;
    vector<uint> friends_rooms;

public:

    explicit TestCase(istream &in) {
        uint n_stations, n_friends;
        in >> n_stations >> n_friends;

        uint n1, n2;
        for (uint i = 0; i < n_stations; ++i) {
            in >> n1 >> n2;
            stations.emplace(Station(i + 1, n1, n2));
        }

        friends_lines = vector<uint>(n_friends);
        for (uint i = 0; i < n_friends; ++i) {
            in >> n1;
            friends_lines[i] = n1;
        }

        friends_rooms = vector<uint>(n_friends);
    }

    uint find_min() const {
        return stations.begin()->u;
    }

    uint find_max() const {
        uint max = 1;  // Lower limit to room number
        for (const auto & s : stations) {
            if (s.v > max)
                max = s.v;
        }
        return max;
    }

    uint count_before(uint station) const {
        uint before = 0;
        for (const auto & s : stations) {
            if (s.u <= station and station <= s.v)
                before += (station - s.u);
            else if (s.v < station) {
                before += s.v - s.u + 1;
            }
        }

        return before;
    }

    void solve() {
        uint min = find_min(), max = find_max();
        for (size_t i = 0; i < friends_lines.size(); ++i) {
            uint f_line = friends_lines[i];
            uint mid, low = min, high = max;
            uint min_maybe, max_maybe;

            while (high > low) {
                mid = (high + low) / 2;
                min_maybe = count_before(mid) + 1;
                max_maybe = count_before(mid + 1);

                if (min_maybe <= f_line and f_line <= max_maybe) {
                    high = mid;
                    break;
                } else if (f_line < min_maybe) {
                    high = mid - 1;
                } else {
                    low = mid + 1;
                }
            }

            friends_rooms[i] = high;
        }
    }

    friend ostream& operator<<(ostream& out, const TestCase& tc);
};

ostream& operator<<(ostream& out, const TestCase& tc) {
    for (int room_number : tc.friends_rooms)
        out << room_number << endl;
    return out;
}

int main() {
    std::ios_base::sync_with_stdio(false); // for better performance

    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i) {
        TestCase tc(cin);
        tc.solve();
        cout << "Case #" << i + 1 << ":" << endl << tc;
    }

    return EXIT_SUCCESS;
}