#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    cin >> n;

    string s;
    for (int i = 0; i < n; ++i) {
        cin >> s;
        cout << "Case #" << i+1 << ": Hello " << s << "!" << endl;
    }
}