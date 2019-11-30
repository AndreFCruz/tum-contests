#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <limits>

#define MAX_SCORE   10

using namespace std;

typedef pair<char, char> comb_t;

struct CharPairHash {
    int operator()(const comb_t& cmb) const {
        return (cmb.first - 'A') * 4 + (cmb.second - 'A'); // set this so that <A,T> == <T,A> (?)
    }
};

/**
 * BioScore from https://www.topcoder.com/community/data-science/data-science-tutorials/greedy-is-good/
 */
class TestCase {
    int n_human, n_mouse;
    vector<string> human_dna;
    vector<string> mouse_dna;

    const unordered_map<comb_t, int, CharPairHash> comb_to_idx = {
            {make_pair('A', 'A'), 0},
            {make_pair('C', 'C'), 1},
            {make_pair('T', 'T'), 2},
            {make_pair('G', 'G'), 3},
            {make_pair('A', 'C'), 4},
            {make_pair('C', 'A'), 4},
            {make_pair('A', 'T'), 5},
            {make_pair('T', 'A'), 5},
            {make_pair('A', 'G'), 6},
            {make_pair('G', 'A'), 6},
            {make_pair('C', 'T'), 7},
            {make_pair('T', 'C'), 7},
            {make_pair('C', 'G'), 8},
            {make_pair('G', 'C'), 8},
            {make_pair('T', 'G'), 9},
            {make_pair('G', 'T'), 9},
    };

public:
    explicit TestCase(istream& in) {
        in >> n_human >> n_mouse;
        for (int i = 0; i < n_human; ++i) {
            string s;
            in >> s;
            human_dna.push_back(s);
        }

        for (int i = 0; i < n_mouse; ++i) {
            string s;
            in >> s;
            mouse_dna.push_back(s);
        }
    }

    static long calc_homology_score(const vector<int> freqs, const vector<int> score) {
        assert(freqs.size() == score.size());
        long sum = 0;
        for (size_t i = 0; i < freqs.size(); ++i)
            sum += freqs[i] * score[i];
        return sum;
    }

    string solve() {
        // Get frequency of combinations
        vector<int> combinations(10, 0); // 10 combinations (disregarding order, e.g. AC == CA)
        for (int h = 0; h < n_human; ++h) {
            for (int m = 0; m < n_mouse; ++m) {
                for (size_t i = 0; i < human_dna[h].size(); ++i) {
                    comb_t cmb = make_pair(human_dna[h][i], mouse_dna[m][i]);
                    combinations[comb_to_idx.at(cmb)] += 1;
                }
            }
        }

        // Order non-diagonal frequencies in descending order
        // (it doesn't matter which combination has a given frequency, only that it exists)
        sort(combinations.begin() + 4, combinations.end(), greater<int>());

        long best_score = 0;
        vector<int> score_matrix(10);
        // Test all combination scores to maximize overall score
        for (int a_score = 1; a_score <= MAX_SCORE; ++a_score) {
            for (int c_score = 1; c_score <= MAX_SCORE; ++c_score) {
                for (int t_score = 1; t_score <= MAX_SCORE; ++t_score) {
                    for (int g_score = 1; g_score <= MAX_SCORE; ++g_score) {

                        // Diagonal scores must be positive
                        score_matrix[0] = a_score;
                        score_matrix[1] = c_score;
                        score_matrix[2] = t_score;
                        score_matrix[3] = g_score;

                        // Remaining scores should maximize similarity between human and mouse DNA
                        // such that sum of all entries equals 0
                        // NOTE non-diagonal entries count x2 (as they appear twice in scoring matrix)

                        // -> attribute highest score to most frequent combinations
                        score_matrix[4] = 10;   // +20
                        score_matrix[5] = 10;   // +20

                        // equalizes diagonal scores (always in range [-10, 10]):
                        score_matrix[6] = 10 - ((score_matrix[0] + score_matrix[1] + score_matrix[2] + score_matrix[3]) / 2);

                        // -> attribute lowest score to least frequent combinations
                        score_matrix[7] = -10;  // -20
                        score_matrix[8] = -10;  // -20
                        score_matrix[9] = -10;  // -20

                        // Calc. score with this matrix
                        long curr_score = calc_homology_score(combinations, score_matrix);
                        if (curr_score > best_score)
                            best_score = curr_score;
                    }
                }
            }
        }

        return to_string(best_score);
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
