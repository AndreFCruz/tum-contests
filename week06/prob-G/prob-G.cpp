#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

#define LEA         'L'
#define TOOL        'T'
#define WALKABLE    '_'
#define NON_WALKABLE '#'

using namespace std;

class TestCase {
    int width, depth;
    vector< vector<char> > map;

    int n_tools = 0;
    pair<int, int> lea_pos;

public:
    explicit TestCase(istream& in) {
        in >> width >> depth;

        map.resize(depth, vector<char>(width));
        for (int row = 0; row < depth; ++row) {
            for (int col = 0; col < width; ++col) {
                in >> map[row][col];
                if (map[row][col] == TOOL)
                    ++n_tools;
                else if (map[row][col] == LEA) {
                    lea_pos = make_pair(row, col);
                    map[row][col] = WALKABLE;
                }
            }
        }
    }

    static void print_board(const vector<vector<char>>& map, const vector<vector<bool>>& used) {
        cout << endl;
        for (size_t row = 0; row < map.size(); ++row) {
            for (size_t col = 0; col < map[row].size(); ++col) {
                if (used[row][col]) {
                    assert(map[row][col] != NON_WALKABLE);
                    if (map[row][col] == TOOL) cout << TOOL;
                    else cout << ".";
                } else {
                    cout << map[row][col];
                }
            }
            cout << endl;
        }
    }

    bool generate_and_test_aux(const vector<vector<char>>& map, vector<vector<bool>>& used, int next_row, int next_col, int grabbed_tools) {
        assert(next_row >= 0 and next_row < depth and next_col >= 0 and next_col < width);

        if (!used[next_row][next_col] and map[next_row][next_col] != NON_WALKABLE) {
            used[next_row][next_col] = true;  // used
            if (generate_and_test(map, used, next_row, next_col, grabbed_tools + (map[next_row][next_col] == TOOL ? 1 : 0)))
                return true;
            used[next_row][next_col] = false; // unused
        }
        return false;
    }

    bool generate_and_test(const vector<vector<char>>& map, vector<vector<bool>>& used, int lea_row, int lea_col, int grabbed_tools) {
        if (grabbed_tools == this->n_tools) {
//            print_board(map, used);
            return true; // No need for testing candidate solution as all paths are appropriately pruned thus far
        }

        // Move upwards
        if (lea_row - 1 >= 0 and generate_and_test_aux(map, used, lea_row - 1, lea_col, grabbed_tools))
            return true;

        // Move downwards
        if (lea_row + 1 < this->depth and generate_and_test_aux(map, used, lea_row + 1, lea_col, grabbed_tools))
            return true;

        // Move leftwards
        if (lea_col - 1 >= 0 and generate_and_test_aux(map, used, lea_row, lea_col - 1, grabbed_tools))
            return true;

        // Move rightwards
        if (lea_col + 1 < this->width and generate_and_test_aux(map, used, lea_row, lea_col + 1, grabbed_tools))
            return true;

        return false;
    }

    string solve() {
        vector<vector<bool>> used(this->depth, vector<bool>(this->width, false));
        used[lea_pos.first][lea_pos.second] = true;

        return generate_and_test(this->map, used, this->lea_pos.first, this->lea_pos.second, 0) ? "yes" : "no";
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
