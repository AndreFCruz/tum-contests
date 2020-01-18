#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits.h>


using namespace std;

// https://www.geeksforgeeks.org/longest-common-subsequence-dp-4/
// its like the LCS
string left_rotate(string s, int rot){
	string res = s;
	reverse(res.begin(), res.begin()+rot);
	reverse(res.begin()+rot, res.end());
	reverse(res.begin(), res.end());
	return res;
}

int solve_dp( string b1, string b2, int sb1, int sb2 )  
{  
	int max_possible = 0;

	// initial matrix
	vector<vector<int>> dp_sol = vector<vector<int>>(sb1 + 1, vector<int>(sb2 + 1, 0));

	// building the matrix
	for (int s_i = 0; s_i <= sb1; ++s_i){
		for (int s_j = 0; s_j <= sb2; ++s_j){
			if (s_i == 0 || s_j == 0){
				dp_sol[s_i][s_j] = 0;
			} 
			else if (b1[s_i - 1] == b2[s_j - 1]){
				dp_sol[s_i][s_j] = dp_sol[s_i - 1][s_j - 1] + 1;
			} else {
				dp_sol[s_i][s_j] = max(dp_sol[s_i-1][s_j], dp_sol[s_i][s_j-1]);
			}
		}
	}
	
	
	for ( int s_i = sb1/2 + 1; s_i <= sb1; ++s_i ){
		for ( int s_j = sb2/2 + 1; s_j <= sb2; ++s_j ){
			max_possible = dp_sol[s_i][s_j] - dp_sol[s_i - sb1/2][s_j - sb2/2];
		}
	}

	//max_possible = max(max_possible, dp_sol[sb1][sb2]);

	// }

	for (auto a: dp_sol){
		for (auto b: a){
			cout << " " << b;
		}
		cout << endl;
	}

	cout << endl;
	cout << endl;

    return max_possible;  
}  

int main () {

	std::ios_base::sync_with_stdio(false);

	int num_tests;
	cin >> num_tests;
	cin.ignore();

	// loop over all test cases
	for ( int i = 1; i <= num_tests ; i ++) {
		// read input 
		string bracelet_1, bracelet_2;
		cin >> bracelet_1 >> bracelet_2;
		
		
		int resulting_max_len = INT_MIN;
		
		string b1_rotation = bracelet_1 + bracelet_1;
		string b2_rotation = bracelet_2 + bracelet_2;

		string b1 = b1_rotation;
		string b2 = b2_rotation;
		// or 
		//string b1 = bracelet_1;
		//string b2 = bracelet_2;

		// cout << b1 << " " << left_rotate(b1, 2) << endl;

		string b1r = b1;
		reverse(b1r.begin(), b1r.end());
		string b2r = b2;
		reverse(b2r.begin(), b2r.end());
		

		resulting_max_len = max(resulting_max_len, solve_dp(b1, b2, b1.size(), b2.size()));
		resulting_max_len = max(resulting_max_len, solve_dp(b1, b2r, b1.size(), b2r.size()));
		resulting_max_len = max(resulting_max_len, solve_dp(b1r, b2, b1r.size(), b2.size()));
		resulting_max_len = max(resulting_max_len, solve_dp(b1r, b2r, b1r.size(), b2r.size()));

		// resulting_max_len = min<int>(resulting_max_len, bracelet_1.size());
		// resulting_max_len = min<int>(resulting_max_len, bracelet_2.size());

		cout << "Case #" << i << ": " << resulting_max_len << endl;
	}

	return 0;
}