#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string find_replace(string s, string oldpttrn, string newpttrn) {
    size_t i = 0;
    while (i < s.size()) {
        i = s.find(oldpttrn, i);
        if (i == s.npos) break;
        s.replace(i, oldpttrn.size(), newpttrn);
        i += newpttrn.size() - oldpttrn.size();
    }

    return s;
}

string process_str(string s) {
    s = find_replace(s, "enten", "ierende");
    s = find_replace(s, "entin", "ierende");
    s = find_replace(s, "ent", "ierender");
    return s;
}

void handle_case() {
    int n;
    string s;

    cin >> n; cin.get(); // extract number and ignore line break
    for (int i = 0; i < n; ++i) {
        getline(cin, s);
        cout << process_str(s) << endl;
    }
}

int main() {
    int num_cases;
    cin >> num_cases;

    for (int i = 0; i < num_cases; ++i) {
        cout << "Case #" << i+1 << ":\n";
        handle_case();
    }
}
