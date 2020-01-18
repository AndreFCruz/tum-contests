#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <limits>
#include <unordered_set>

using namespace std;


class TestCase {
    long n_facilities;
    long n_customers;

    vector<long> rental_cost;           // rental cost per facility
    vector<vector<long>> delivery_cost; // delivery cost for each facility for each customer

public:
    explicit TestCase(istream& in) {
        in >> n_facilities >> n_customers;

        rental_cost.resize(n_facilities);
        for (long i = 0; i < n_facilities; ++i) {
            in >> rental_cost[i];
        }

        delivery_cost.resize(n_facilities, vector<long>(n_customers));
        for (long fac = 0; fac < n_facilities; ++fac) {
            for (long cust = 0; cust < n_customers; ++cust) {
                in >> delivery_cost[fac][cust];
            }
        }
    }

    /**
     * Performs local search from the provided valid solution
     * @param used a valid solution corresponding to the ids of the used facilities
     * @param unused ids of unused facilities
     * @param cust_cost delivery cost for each customer
     * @param cust_fac facility serving each customer
     * @param curr_total total cost of this solution
     * @return cost for new improved solution (<= provided curr_total)
     */
    long local_search(unordered_set<long>& used, vector<long>& unused, vector<long>& cust_cost, vector<long>& cust_fac, vector<long>& n_custs_per_fac, long curr_total) {

        // For every unused facility, check improvement by adding it to the solution
        long min_cost_diff = 0;
        long min_cost_fac = -1, min_cost_fac_idx = -1;
        for (size_t i = 0; i < unused.size(); ++i) { // Check all unused facilities or use first facility that improves solution ?
            long fac = unused[i];
            long cost_diff = rental_cost[fac];

            for (long cust = 0; cust < n_customers; ++cust) {
                if (delivery_cost[fac][cust] < cust_cost[cust])
                    cost_diff += delivery_cost[fac][cust] - cust_cost[cust];
            }

            if (cost_diff < min_cost_diff) {
                min_cost_diff = cost_diff;
                min_cost_fac = fac;
                min_cost_fac_idx = i;
//                break; // break here? use first improved solution found ?
            }
        }

        // If any facility improves solution, use it
        if (min_cost_fac == -1) return curr_total; // local optimum reached

        // Create new solution by adding the min_cost_fac
        used.insert(min_cost_fac);
        unused.erase(unused.begin() + min_cost_fac_idx);

        for (long cust = 0; cust < n_customers; ++cust) {
            if (delivery_cost[min_cost_fac][cust] < cust_cost[cust]) {
                long prev_cust_fac = cust_fac[cust];
                cust_cost[cust] = delivery_cost[min_cost_fac][cust];
                cust_fac[cust] = min_cost_fac;

                // Remove from previous facility
                n_custs_per_fac[prev_cust_fac] -= 1;
                if (n_custs_per_fac[prev_cust_fac] <= 0) {
                    assert(n_custs_per_fac[prev_cust_fac] == 0);
                    min_cost_diff -= rental_cost[prev_cust_fac];
                    unused.push_back(prev_cust_fac);
                    used.erase(prev_cust_fac);
                }

                // Add to new one
                n_custs_per_fac[min_cost_fac] += 1;
            }
        }

        return curr_total + min_cost_diff;
    }

    void solve() {
        // Initial valid solution: one facility to serve all customers

        long min_total_cost = numeric_limits<long>::max(), min_cost_fac = numeric_limits<long>::max();
        // Select facility with least cost from all facilities
        for (long fac = 0; fac < n_facilities; ++fac) {
            long total_cost = rental_cost[fac];
            for (long cust = 0; cust < n_customers; ++cust) {
                total_cost += delivery_cost[fac][cust];
            }

            if (total_cost < min_total_cost) {
                min_total_cost = total_cost;
                min_cost_fac = fac;
            }
        }

        // Build unused vector
        vector<long> unused;
        for (long fac = 0; fac < n_facilities; ++fac) {
            if (fac != min_cost_fac)
                unused.push_back(fac);
        }

        // Build used vector
        unordered_set<long> used = {min_cost_fac};

        // Build vector for current cost per customer
        vector<long> cost_per_customer(delivery_cost[min_cost_fac].begin(), delivery_cost[min_cost_fac].end());

        // Build vector for current facility for customer
        vector<long> facility_for_customer(n_customers, min_cost_fac);

        // Build vector for current number of customers per facility
        vector<long> custs_per_fac(n_facilities, 0);
        custs_per_fac[min_cost_fac] = n_customers;

        // Perform local search from this solution
        long old_cost, new_cost = min_total_cost;
        do {
            old_cost = new_cost;
            new_cost = local_search(used, unused, cost_per_customer, facility_for_customer, custs_per_fac, old_cost);
        } while (new_cost < old_cost);

        // Get customers per facility (invert facility_for_customer)
        vector<vector<long>> customers_per_facility(n_facilities);
        long current_cost_check = 0;
        for (long cust = 0; cust < n_customers; ++cust) {
            customers_per_facility[facility_for_customer[cust]].push_back(cust);
            current_cost_check += delivery_cost[facility_for_customer[cust]][cust];
        }

        for (long fac : used)
            current_cost_check += rental_cost[fac];
        assert(current_cost_check == new_cost);

        // Print solution
        cout << new_cost << endl;
        for (size_t fac = 0; fac < customers_per_facility.size(); ++fac) {
            if (customers_per_facility[fac].empty()) continue;
            cout << fac + 1 << " ";
            for (long cust : customers_per_facility[fac])
                cout << cust + 1 << " ";
            cout << endl;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    long num_cases;
    cin >> num_cases;
    for (long i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": ";
        TestCase(cin).solve();
    }

    return EXIT_SUCCESS;
}
