#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <queue>

typedef unsigned long ulong;

using namespace std;

class TestCase {
    ulong n_trees;
    ulong n_saws;
    vector<ulong> times;

public:
    explicit TestCase(istream& in) {
        in >> n_trees >> n_saws;

        times.resize(n_trees);
        for (ulong i = 0; i < n_trees; ++i) {
            in >> times[i];
        }
    }

    ulong greedy_scheduling() {
        // 1. Order trees from largest to smallest
        sort(times.begin(), times.end(), greater<ulong>());

        // 2. Attribute largest tree to the least busy saw
        priority_queue<ulong, vector<ulong>, greater<ulong>> pq_saws; // used time per saw
        for (ulong i = 0; i < this->n_saws; ++i)
            pq_saws.push(0);

        ulong max_saw_time = 0;

        for (ulong i = 0; i < times.size(); ++i) {
            ulong tree_time = times[i];
            ulong saw_time = pq_saws.top();

            // Increase key for this saw in max-heap
            pq_saws.pop();
            ulong new_saw_time = saw_time + tree_time;
            pq_saws.push(new_saw_time);
            if (new_saw_time > max_saw_time)
                max_saw_time = new_saw_time;
        }

        return max_saw_time;
    }

    string solve() {
        return to_string(greedy_scheduling());
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    ulong num_cases;
    cin >> num_cases;
    for (ulong i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
