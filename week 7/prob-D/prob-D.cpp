#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <queue>

typedef unsigned int uint;

using namespace std;

class TestCase {
    uint n_trees;
    uint n_saws;
    vector<uint> times;

public:
    explicit TestCase(istream& in) {
        in >> n_trees >> n_saws;

        times.resize(n_trees);
        for (uint i = 0; i < n_trees; ++i) {
            in >> times[i];
        }
    }

    uint greedy_scheduling() {
        // 1. Order trees from largest to smallest
        sort(times.begin(), times.end(), greater<uint>());

        // 2. Attribute largest tree to the least busy saw
        priority_queue<uint> pq_saws; // used time per saw
        for (uint i = 0; i < this->n_saws; ++i)
            pq_saws.push(0);

        uint max_saw_time = 0;

        for (uint i = 0; i < times.size(); ++i) {
            int tree_time = times[i];
            int saw_time = pq_saws.top();

            // Increase key for this saw in max-heap
            pq_saws.pop();
            int new_saw_time = saw_time + tree_time;
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

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
