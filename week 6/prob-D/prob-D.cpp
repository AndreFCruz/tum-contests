#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

using namespace std;

void print_chessboard(const vector<vector<bool>> &sol, int n) {
    for (int i=1; i<=n; ++i)
    {
        for (int j=1; j<=n; ++j)
        {
            if (sol[i][j])
                cout << 'x';
            else
                cout << '.';
        }
        cout << "\n";
    }
}

bool check_candidate_solution(const vector<vector<bool>> &sol, int i, int j, int n) {
    // vertical up
    for (int k=1; k<i; ++k)
        if (sol[k][j])
            return false;

    // vertical down
    for (int k=i+1; k<=n; ++k)
        if (sol[k][j])
            return false;

    // diagonal left-up
    for (int k=1; k<=min(i-1, j-1); ++k)
        if (sol[i-k][j-k])
            return false;

    // diagonal right-up
    for (int k=1; k<=min(i-1, n-j); ++k)
        if (sol[i-k][j+k])
            return false;

    // diagonal right-down
    for (int k=1; k<=min(n-i, j-1); ++k)
        if (sol[i+k][j-k])
            return false;

    // diagonal left-down
    for (int k=1; k<=min(n-i, n-j); ++k)
        if (sol[i+k][j+k])
            return false;

    return true;
}


bool check_chessboard(vector<vector<bool>> board, int n) {
    for (int row=1; row<=n; ++row)
    {
        // check that there is at most one queen
        int queens = 0;
        for (int column=1; column<=n; ++column)
        {
            if (board[row][column])
            {
                bool res = check_candidate_solution(board, row, column, n);
                if (!res)
                {
                    return false;
                }
                ++queens;
            }
        }
        if (queens > 1)
            return false;
    }

    return true;
}

void find_solutions_for_row(const vector<vector<bool>> &currentSolution, int row, int n, vector<vector<vector<bool>>> &allSolutions) {
    // assume only one queen per row
    for (int column=1; column<=n; ++column)
    {
        vector<vector<bool>> possibleSolution (currentSolution);
        possibleSolution[row][column] = true;
        bool works = check_candidate_solution(possibleSolution, row, column, n);
        if (works)
        {
            allSolutions.push_back(possibleSolution);
        }
    }
}

bool backtrack(const vector<vector<bool>> &currentSolution, int n, priority_queue<int, vector<int>, greater<int> > pq)
{
    // a queen is placed in every row
    if (pq.empty()) {
        print_chessboard(currentSolution, n);
        return true;
    }

    int nextRow = pq.top();
    pq.pop();

    vector<vector<vector<bool>>> nextSolutions;
    find_solutions_for_row(currentSolution, nextRow, n, nextSolutions);

    for (const auto &nextSolution : nextSolutions) {
        bool result = backtrack(nextSolution, n, pq);
        if (result) {
            return true;
        }
    }

    return false;
}

class TestCase {
    int grid_dim;
    vector<vector<bool>> sol;
    vector<bool> rows_available;

public:
    explicit TestCase(istream &in) {
        in >> grid_dim;

        this->sol.resize(grid_dim+1, vector<bool>(grid_dim+1));
        this->rows_available.resize(grid_dim + 1);
        for (int k=1; k<=grid_dim; ++k) {
            rows_available[k] = true;
        }

        for (int i=1; i<=grid_dim; ++i) {
            for (int j=1; j<=grid_dim; ++j) {
                char queen;
                in >> queen;
                if (queen == 'x') {
                    sol[i][j] = true;
                    rows_available[i] = false;
                }
            }
        }
    }

    void solve() {
        if (! check_chessboard(sol, grid_dim)) {
            cout << "impossible\n";
            return;
        }

        // create pq for rows
        priority_queue<int, vector<int>, greater<int> > pq;
        for (int i=1; i<=grid_dim; ++i)
        {
            if (rows_available[i])
            {
                pq.push(i);
            }
        }

        if (! backtrack(sol, grid_dim, pq)) {
            cout << "impossible\n";
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ":\n";
        TestCase(cin).solve();
    }

    return EXIT_SUCCESS;
}

