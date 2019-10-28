#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

using namespace std;

struct Station {
    int id;     // Station's number
    int u;      // Station's room range start
    int v;      // Station's room range end

    Station(int id, int u, int v) : id(id), u(u), v(v) {}
};

struct StationIdCmp {
    bool operator()(const Station& s1, const Station& s2) { return s1.id < s2.id; };
};

class TestCase {
    vector<Station> stations; // Maps station id to its interval
    vector<int> friends_lines;   // maps i-th friend to respective room's line number
    vector<int> friends_rooms;   // answer to the problem (friends' room numbers)

public:

    explicit TestCase(istream &in) {
        int n_stations, n_friends;
        in >> n_stations >> n_friends;

        int n1, n2;
        for (int i = 0; i < n_stations; ++i) {
            in >> n1 >> n2;
            stations.push_back(Station(i + 1, n1, n2));
        }
        // Sort stations by their starting room number
        sort(stations.begin(), stations.end(), [](const Station& s1, const Station& s2) {
            if (s1.u != s2.u) return s1.u < s2.u;
            else return s1.id < s2.id;
        });

        friends_lines = vector<int>(n_friends);
        for (int i = 0; i < n_friends; ++i) {
            in >> n1;
            friends_lines[i] = n1;
        }

        friends_rooms = vector<int>(n_friends);
    }

    void solve() {
        // Friends sorted by their line numbers
        map<int, int> lines_friends;  // inverse relation from friends_lines (this maps lines to friends)
        for (size_t i = 0; i < friends_lines.size(); ++i)
            lines_friends[friends_lines[i]] = i;

        auto begin_it = lines_friends.begin(), end_it = lines_friends.end();
        int current_line = 1, current_room = stations[0].u;
        set<Station, StationIdCmp> active_stations;
        // Populate active_stations
        for (size_t i = 0; i < stations.size(); ++i) {
            if (stations[i].u == stations[0].u)
                active_stations.insert(stations[i]);
        }

        while (begin_it != end_it) {
            int line = begin_it->first, f_idx = begin_it->second;

            // Advance lines until this friend's line is reached, then record room number
            for (auto it = active_stations.begin(); it != active_stations.end(); ++it) {

            }

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