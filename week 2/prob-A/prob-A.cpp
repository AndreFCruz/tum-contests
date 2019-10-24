#include <iostream>
#include <vector>
#include <set>
// #include <unordered_set>
#include <cassert>

using namespace std;


class NotablePerson {
public:
    int id;
    int money;
    set<int> relations;
    int marriage;    // marriage is a subset of this person's relations

    NotablePerson(int id) : id(id) {}
};


class TestCase {
private:
    vector<NotablePerson> people;

public:
    TestCase(istream & in) {
        int a;  // num. notable people
        int b;  // num. family relations
        int c;  // num. marriages
        in >> a >> b >> c;

        // Money
        int n1, n2;
        for (int i = 0; i < a - 1; ++i) {
            people.push_back(NotablePerson(i + 1));
            in >> n1;
            people.back().money = n1;
        }
        people.push_back(NotablePerson(a)); // Insert Jakob Fugger

        // Family relations
        for (int i = 0; i < b; ++i) {
            in >> n1 >> n2;
            people[n1 - 1].relations.insert(n2);
            people[n2 - 1].relations.insert(n1);

            assert(people[n1 - 1].id == n1 and people[n2 - 1].id == n2);
        }

        cout << "hey :D\n";
        // Marriages
        for (int i = 0; i < c; ++i) {
            in >> n1 >> n2;
            people[n1 - 1].marriage = n2;
            people[n2 - 1].marriage = n1;

            people[n1 - 1].relations.insert(n2);
            people[n2 - 1].relations.insert(n1);
        }

        cout << "done processing STDIN" << endl;
    }


// FCK THIS SHIT
    // int depth_first_sum(int idx) {
    //     set<int> s = {idx};
    //     // vector<bool> visited(people.size(), false);
    //     vector<bool> visited(people.size(), false);
    //     int sum = 0;

    //     while (! s.empty()) {
    //         cout << "sum: " << sum << endl;
    //         auto it = s.begin();
    //         int new_idx = *it;
    //         s.erase(it);

    //         if (! visited[new_idx]) {
    //             visited[new_idx] = true;
    //             sum += people[new_idx].money;
    //             s.merge(people[new_idx].relations);
    //         }
    //     }

    //     return sum;
    // }

    TestCase * solve() {
        this->depth_first_sum(0);
        // TODO
        return this;
    }

    friend ostream & operator<<(ostream& out, TestCase& test_case);
};

ostream & operator<<(ostream& out, TestCase& test_case) {
    return out << "TEST TODO ; " << endl;
}

int main() {
    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i) {
        cout << *(TestCase(cin).solve()) << endl;
    }

    return EXIT_SUCCESS;
}