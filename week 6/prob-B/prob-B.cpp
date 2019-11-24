#include <iostream>
#include <vector>
#include <cassert>
#include <numeric>
#include <set>
#include <algorithm>

using namespace std;

struct Dependency {
    int c;  // first chapter's character
    int p;  // first chapter
    int d;  // second chapter's character
    int q;  // second chapter
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

        chapters_per_character.resize(n_characters);
        for (int i = 0; i < n_characters; ++i) {
            in >> chapters_per_character[i];
            n_chapters += chapters_per_character[i];
        }
        assert(n_chapters == reduce(chapters_per_character.begin(), chapters_per_character.end()));

        dependencies.resize(n_chapter_dependencies);
        for (int i = 0; i < n_chapter_dependencies; ++i) {
            in >> dependencies[i].c >> dependencies[i].p >> dependencies[i].d >> dependencies[i].q;
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
        // TODO
        // Check dependencies

    }

    int generate_and_test(vector<int>& character_on_chapter, vector<int>& available, int idx) {
        if (idx == n_chapters)
            return test(character_on_chapter) ? 1 : 0;

        int total = 0;
        for (int i = 0; i < available.size(); ++i) {
            int character = available[i];

            // Already used ?
            if (character == -1)
                continue;

            // Mark as used
            available[i] = -1;

            // Generate new candidate solution
            character_on_chapter[idx] = character;

            // Recursively generate new solutions from this one
            total += generate_and_test(character_on_chapter, available, idx + 1);

            // Mark as not used (Backtrack)
            available[i] = character;
        }

        return total;
    }

    string solve() {
        // Build bag of characters
        multiset<int> bag_of_characters;    // each character appears once for each time a chapter is based on her
        for (int i = 0; i < n_characters; ++i) {
            for (int j = 0; j < chapters_per_character[i]; ++j)
                bag_of_characters.insert(i);
        }

        vector<int> character_on_chapter(n_chapters);

        generate_and_test(character_on_chapter, bag_of_characters, 0);

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
