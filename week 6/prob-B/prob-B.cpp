#include <iostream>
#include <vector>
#include <cassert>
//#include <numeric>
#include <algorithm>

using namespace std;

struct Dependency {
    int c;  // first chapter's character
    int p;  // p-th chapter centered around character c
    int d;  // second chapter's character
    int q;  // q-th chapter centered around character d
    // NOTE p-th chapter centered on c must happen before q-th chapter centered on d
};


class TestCase {
    int n_characters;
    int n_chapter_dependencies;
    vector<int> chapters_per_character; // vec[character_i] = number of chapters centered on character_i
    vector<Dependency> dependencies;

    int n_chapters = 0;
public:
    explicit TestCase(istream& in) {
        in >> n_characters >> n_chapter_dependencies;

        chapters_per_character.resize(n_characters, 0);
        for (int i = 0; i < n_characters; ++i) {
            in >> chapters_per_character[i];
            n_chapters += chapters_per_character[i];
        }
//        assert(n_chapters == reduce(chapters_per_character.begin(), chapters_per_character.end()));
//        assert(n_characters == (int) chapters_per_character.size());

        dependencies.resize(n_chapter_dependencies);
        for (int i = 0; i < n_chapter_dependencies; ++i) {
            in >> dependencies[i].c >> dependencies[i].p >> dependencies[i].d >> dependencies[i].q;
            dependencies[i].c -= 1;
            dependencies[i].d -= 1;
        }

        // Sort dependencies per character
        sort(dependencies.begin(), dependencies.end(), [](const Dependency& d1, const Dependency& d2) {
            if (d1.p != d2.p)
                return d1.p < d2.p;
            else if (d1.q != d2.q)
                return d1.q < d2.q;
            else if (d1.c != d2.c)
                return d1.c < d2.c;
            return d1.d < d2.d;
        });
    }

    bool test(const vector<int>& character_on_chapter) {
        assert(n_chapters == (int) character_on_chapter.size());

        // Check dependencies
        for (const Dependency& dep : this->dependencies) {


            int c_chapters_count = 0;   // count of chapters containing character dep.c
            int d_chapters_count = 0;   // count of chapters containing character dep.d
            for (int i = 0; i < n_chapters; ++i) {
                if (character_on_chapter[i] == dep.c)
                    ++c_chapters_count;
                else if (character_on_chapter[i] == dep.d)
                    ++d_chapters_count;

                if (c_chapters_count == dep.p) {
                    if (d_chapters_count >= dep.q)
                        return false;   // dependency fails
                    else
                        break;          // dependency asserted
                }
            }
        }

        // Print solution that's being tested
//        for (int c : character_on_chapter)
//            cout << c << " ";
//        cout << endl;
        return true;
    }

    int generate_and_test(vector<int>& character_on_chapter, vector<int>& available, int idx) {
        if (idx == n_chapters)
            return test(character_on_chapter) ? 1 : 0;

        int total = 0;
        for (int character = 0; character < this->n_characters; ++character) { // This happens at most 6 times, as there are at most 6 characters
            int availableChapters = available[character];

            // Can't have same character in two sequential chapters -- PRUNE
            if (idx > 0 and character_on_chapter[idx - 1] == character)
                continue;

            // No chapters left with this character
            if (availableChapters <= 0) {
                assert(availableChapters == 0);
                continue;
            }

            // Mark as used
            available[character] -= 1;

            // Generate new candidate solution
            character_on_chapter[idx] = character;

            // Recursively generate new solutions from this one
            total += generate_and_test(character_on_chapter, available, idx + 1);

            // Mark as not used (Backtrack)
            available[character] += 1;
        }

        return total;
    }

    string solve() {
        // Available chapters centered on a given character
        vector<int> available(chapters_per_character.begin(), chapters_per_character.end());

        // Candidate solution
        vector<int> character_on_chapter(n_chapters);

        int total = generate_and_test(character_on_chapter, available, 0);
        return to_string(total);
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
