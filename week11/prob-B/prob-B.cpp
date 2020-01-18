#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cmath>

using namespace std;


struct SegmentTreeNode {
    int left = 0, right = 0, value = 0, lazy = 0;
};


class SegmentTreeLazy {
private:
    int height;
    int size;
    SegmentTreeNode* nodes;
//    vector<SegmentTreeNode> nodes;

    static int buildSegmentTree(SegmentTreeLazy* t, int* a, int curr_idx, int left, int right) {
        t->nodes[curr_idx].left = left;
        t->nodes[curr_idx].right = right;
        if (left == right) {
            t->nodes[curr_idx].value = a[left];
            return t->nodes[curr_idx].value;
        }

        int mid = (left + right) / 2;
        t->nodes[curr_idx].value = \
            buildSegmentTree(t, a, 2 * curr_idx + 1, left, mid) + \
            buildSegmentTree(t, a, 2 * curr_idx + 2, mid + 1, right);
        return t->nodes[curr_idx].value;
    }

public:
    SegmentTreeLazy(int* input_array, int array_size) {
        height = ceil(log2(array_size));
        size = pow(2, height + 1);
        nodes = new SegmentTreeNode[size]();
//        nodes = vector<SegmentTreeNode>(size);
        SegmentTreeLazy::buildSegmentTree(this, input_array, 0, 0, array_size - 1);
    }

    ~SegmentTreeLazy() {
//        delete[] nodes;
    }

    int getSum(int left, int right) {
        return getSum(0, left, right);
    }

    int getSum(int curr_idx, int left, int right) {
        if (left > nodes[curr_idx].right or right < nodes[curr_idx].left)
            return 0;

        propagate(curr_idx);
        if (left <= nodes[curr_idx].left and nodes[curr_idx].right <= right)
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

//        string s;
//        getline(in, s); // clean dummy new line
//        for (int i = 0; i < k_queries; ++i)
//        {
//            getline(in, s);
//            queries.push_back(s);
//        }
    }

    int solve() {
        int * arr = new int[n_glasses]();
        SegmentTreeLazy st(arr, n_glasses);

        int ans = 0;
        for (int k = 0; k < k_queries; ++k) {
            char query_type;
            cin >> query_type;

            if (query_type == 'q') {
                int target_glass;
                cin >> target_glass;
                ans += st.getSum(target_glass-1, target_glass-1);
                ans %= 1000000007;
            }
            else if (query_type == 'i')
            {
                int l, r, v;
                cin >> l >> r >> v;
                st.rangeAdd(l-1, r-1, v);
            }
            else
                throw invalid_argument("Invalid query argument");
        }

        delete[] arr;
        return ans;
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
