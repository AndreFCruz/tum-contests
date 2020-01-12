#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

using namespace std;

// Trie implementation from: https://www.geeksforgeeks.org/trie-insert-and-search/
const int ALPHABET_SIZE = 26;

// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];

    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode()
{
    struct TrieNode *pNode =  new TrieNode;

    pNode->isEndOfWord = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;

    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}

// Returns true if key presents in trie, else
// false
bool search(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

class TestCase {
    int n_rounds, w_words;
    vector<string> words;
    TrieNode* trie_root;

public:
    explicit TestCase() {
        cin >> n_rounds >> w_words;
        cin.ignore();

        trie_root = getNode();
        string s;
        for (int i = 0; i < w_words; ++i) {
            getline(cin, s);
            words.push_back(s);
            insert(trie_root, s);
        }
    }

    /**
     * Checks if maximizing player is guaranteed to win the current node
     * @param node
     * @param is_max
     * @return
     */
    bool minimax(const TrieNode* node, bool is_max) {
        // Is terminal/leaf node ?
        if (node->isEndOfWord)
            return is_max;

        if (is_max) {
            for (const TrieNode* child : node->children) {
                if (child != NULL and minimax(child, not is_max))
                    return true;
            }
            return false;
        }
        else {
            for (const TrieNode* child : node->children) {
                if (child != NULL and !minimax(child, !is_max))
                    return false;
            }
            return true;
        }
    }

    /**
     * Solve the game under a specific scenario
     * @param lea_begins Lea begins in round 1
     * @param winner_starts the winner of round k begins in the round k+1
     */
    void solve(bool lea_begins, bool winner_starts) {
        // TODO use arguments
        if (minimax(trie_root, true))
            cout << "victory\n";
        else
            cout << "defeat\n";
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ":" << endl;
        TestCase().solve(true, true);
        TestCase().solve(true, false);
        TestCase().solve(false, true);
        TestCase().solve(false, false);
    }

    return EXIT_SUCCESS;
}
