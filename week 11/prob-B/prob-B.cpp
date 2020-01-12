#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;


struct SegmentTreeNode {
    int left, right, value, lazy;
};


class SegmentTreeLazy {
private:
    int size;
    SegmentTreeNode* nodes;

    static int buildSegmentTree(SegmentTreeLazy* t, int* a, int curr_idx, int left, int right) {
        t->nodes[curr_idx].left = left;
        t->nodes[curr_idx].right = right;
        if (left == right) {
            t->nodes[curr_idx].value = a[left];
            return t->nodes[curr_idx].value;
        }

        int mid = (left + right) / 2;
        t->nodes[curr_idx].value = \
            buildSegmentTree(t, a, 2 * curr_idx + 1, left, mid) +
                                   buildSegmentTree(t, a, 2 * curr_idx + 2, mid + 1, right);
        return t->nodes[curr_idx].value;
    }

public:
    SegmentTreeLazy(int* input_array, int size) : size(size) {
        nodes = new SegmentTreeNode[size]();
        SegmentTreeLazy::buildSegmentTree(this, input_array, 0, 0, size - 1);
    }

    ~SegmentTreeLazy() {
        delete[] nodes;
    }

    int getSum(int left, int right) {
        return getSum(0, left, right);
    }

    int getSum(int curr_idx, int left, int right) {
        if (left > nodes[curr_idx].right or right < nodes[curr_idx].left)
            return 0;
        else if (left <= nodes[curr_idx].left and nodes[curr_idx].right <= right)
            return nodes[curr_idx].value;

        return \
            getSum(2 * curr_idx + 1, left, right) + \
            getSum(2 * curr_idx + 2, left, right);
    }

    void add(int curr_idx, int update_idx, int value) {
        if (update_idx < nodes[curr_idx].left or update_idx > nodes[curr_idx].right)
            return;
        nodes[curr_idx].value += value;
        if (nodes[curr_idx].left != nodes[curr_idx].right) {
            add(2 * curr_idx + 1, update_idx, value);
            add(2 * curr_idx + 2, update_idx, value);
        }
    }

    void rangeAdd(int left, int right, int value) {
        rangeAdd(0, left, right, value);
    }

    void rangeAdd(int curr_idx, int left, int right, int value) {
        propagate(curr_idx);
        if (left > nodes[curr_idx].right or right < nodes[curr_idx].left)
            return;

        if (left <= nodes[curr_idx].left and nodes[curr_idx].right <= right) {
            nodes[curr_idx].lazy += value;
            propagate(curr_idx);
        }
        else if (nodes[curr_idx].left != nodes[curr_idx].right) {
            rangeAdd(2 * curr_idx + 1, left, right, value);
            rangeAdd(2 * curr_idx + 2, left, right, value);
            nodes[curr_idx].value = \
                nodes[2 * curr_idx + 1].value + \
                nodes[2 * curr_idx + 2].value;
        }
    }

    void propagate(int curr_idx) {
        nodes[curr_idx].value += (nodes[curr_idx].right - nodes[curr_idx].left + 1) * nodes[curr_idx].lazy;
        if (nodes[curr_idx].left != nodes[curr_idx].right){
            nodes[2 * curr_idx + 1].lazy += nodes[curr_idx].lazy;
            nodes[2 * curr_idx + 2].lazy += nodes[curr_idx].lazy;
        }
        nodes[curr_idx].lazy = 0;
    }

};



class TestCase
{
    int n_glasses;
    int k_queries;
    vector<string> queries;

public:
    explicit TestCase(istream &in)
    {
        in >> n_glasses >> k_queries;

        string s;
        getline(in, s); // clean dummy new line
        for (int i = 0; i < k_queries; ++i)
        {
            getline(in, s);
            queries.push_back(s);
        }
    }

    int solve() {

//        int ans = 0;
//        for (string s : queries) {
//            if (s[0] == 'q') {
//                int target_glass = atoi(s.substr(2).c_str());
//                ans += getSum(n_glasses, target_glass, target_glass);
//            }
//            else if (s[0] == 'i')
//            {
//                stringstream str_stream(s.substr(2));
//                int l, r, v;
//                str_stream >> l >> r >> v;
//                updateRange(n_glasses, l, r, v);
//            }
//            else
//                throw invalid_argument("Invalid query argument");
//        }
//
//        return ans % 1000000007;
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i)
    {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
