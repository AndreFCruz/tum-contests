#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

using namespace std;

/* boolean
 ___       0
|	|   1    2
|	|   
 ___       3
|	|    
|	|   4     5
 ___       6
*/
constexpr uint8_t n0 = (1 << 0) + (1 << 1) + (1 << 2) + (0 << 3) + (1 << 4) + (1 << 5) + (1 << 6);
constexpr uint8_t n1 = (0 << 0) + (0 << 1) + (1 << 2) + (0 << 3) + (0 << 4) + (1 << 5) + (0 << 6);
constexpr uint8_t n2 = (1 << 0) + (0 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (0 << 5) + (1 << 6);
constexpr uint8_t n3 = (1 << 0) + (0 << 1) + (1 << 2) + (1 << 3) + (0 << 4) + (1 << 5) + (1 << 6);
constexpr uint8_t n4 = (0 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (0 << 4) + (1 << 5) + (0 << 6);
constexpr uint8_t n5 = (1 << 0) + (1 << 1) + (0 << 2) + (1 << 3) + (0 << 4) + (1 << 5) + (1 << 6);
constexpr uint8_t n6 = (1 << 0) + (1 << 1) + (0 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6);
constexpr uint8_t n7 = (1 << 0) + (0 << 1) + (1 << 2) + (0 << 3) + (0 << 4) + (1 << 5) + (0 << 6);
constexpr uint8_t n8 = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6);
constexpr uint8_t n9 = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (0 << 4) + (1 << 5) + (1 << 6);

unordered_map <int, int> gather_results;

uint8_t convert_char(char c){
	switch (c)
	{
	case '0':
		return n0;
	case '1':
		return n1;
	case '2':
		return n2;
	case '3':
		return n3;
	case '4':
		return n4;
	case '5':
		return n5;
	case '6':
		return n6;
	case '7':
		return n7;
	case '8':
		return n8;		
	case '9':
		return n9;
	default:
		cout << "error char" << endl;
		return -1;
	}
}

bool is_valid_digit(uint8_t x){
	if ( x != n0 && x != n1 && x != n2 && x != n3 && x != n4 && x != n5 && x != n6 && x != n7 && x != n8 && x != n9)
		return false;
	return true;
}

bool test(uint32_t x){
	uint8_t hd = (x << (32 - 28)) >> 25;
	uint8_t hu = (x << (32 - 21)) >> 25;
	uint8_t md = (x << (32 - 14)) >> 25;
	uint8_t mu = (x << (32 - 7)) >> 25;

	if ( hd != n0 && hd != n1 && hd != n2)
		return false;

	if ( hd == n2 && ( hu != n0 && hu != n1 && hu != n2 && hu != n3))
		return false;
	
	if (!is_valid_digit(hu))
		return false;

	if ( md != n0 && md != n1 && md != n2 && md != n3 && md != n4 && md != n5)
		return false;

	if (!is_valid_digit(mu))
		return false;

	return true;
}

int convert_clock_i(uint8_t val){
	switch (val)
	{
	case n0:
		return 0;
	case n1:
		return 1;
	case n2:
		return 2;
	case n3:
		return 3;
	case n4:
		return 4;
	case n5:
		return 5;
	case n6:
		return 6;
	case n7:
		return 7;
	case n8:
		return 8;
	case n9:
		return 9;
	default:
		cout << "error clock" << endl;
		return -1;
	}
}

string convert_clock(uint8_t val){
	switch (val)
	{
	case n0:
		return "0";
	case n1:
		return "1";
	case n2:
		return "2";
	case n3:
		return "3";
	case n4:
		return "4";
	case n5:
		return "5";
	case n6:
		return "6";
	case n7:
		return "7";
	case n8:
		return "8";
	case n9:
		return "9";
	default:
		cout << "error clock" << endl;
		return "";
	}
}

string print_clock(uint32_t x){
	uint8_t hd = (x << (32 - 28)) >> 25;
	uint8_t hu = (x << (32 - 21)) >> 25;
	uint8_t md = (x << (32 - 14)) >> 25;
	uint8_t mu = (x << (32 - 7)) >> 25;
	return convert_clock(hd) + convert_clock(hu) + ":" + convert_clock(md) + convert_clock(mu);
}

int n_minutes_clock(uint32_t x){
	uint8_t hd = (x << (32 - 28)) >> 25;
	uint8_t hu = (x << (32 - 21)) >> 25;
	uint8_t md = (x << (32 - 14)) >> 25;
	uint8_t mu = (x << (32 - 7)) >> 25;
	return convert_clock_i(hd)*600 + convert_clock_i(hu)*60 + convert_clock_i(md)*10 + convert_clock_i(mu);
}

void print_minutes_clock(int n_min){
	int hours = n_min / 60;
	int minutes = n_min % 60;
	cout << setfill('0') << setw(2) << hours << ":" << setfill('0') << setw(2) << minutes << endl;
	return;
}

bool generate_and_test(uint32_t & clck, int idx, uint32_t bmask, int offset) {

	if (idx == -1){
		if ( test(clck) ){
			int n_minutes = n_minutes_clock(clck);
			if (offset == 0){
				gather_results.insert(make_pair(n_minutes, 1));
			} else {
				int base_time = n_minutes - offset;
				if (base_time < 0)
					base_time += 24*60;
				auto iter = gather_results.find(base_time);
				if (iter != gather_results.end())
					gather_results[iter->first] = iter->second + 1;
			}
			return true;
		}
		return false;
	} 

	if ((clck >> idx) & 1) // this bit is set
		return generate_and_test(clck, idx - 1, bmask, offset);

	if ((bmask >> idx) & 1)
		return generate_and_test(clck, idx - 1, bmask, offset);

	bool flag = false;

	clck = clck | (1 << idx);
	flag = generate_and_test(clck, idx - 1, bmask, offset) || flag;

	clck = clck & ~(1 << idx);
	flag = generate_and_test(clck, idx - 1, bmask, offset) || flag;

	return flag;
}

int main () {

	std::ios_base::sync_with_stdio(false);

	int num_tests;
	cin >> num_tests;
	cin.ignore();

	// loop over all test cases
	for ( int i = 1; i <= num_tests ; i ++) {
		gather_results = unordered_map<int, int>();

		// read input 
		int n_clcks;
		cin >> n_clcks;

		uint32_t bit_mask = 0; // all zeros
		vector<uint32_t> all_recorded_times = vector<uint32_t>(n_clcks);
		// read every line
		for (int j = 0; j < n_clcks; j++){
			uint8_t hd, hu, md, mu;
			char val;
			cin >> val;
			hd = convert_char(val);
			cin >> val;
			hu = convert_char(val);
			cin >> val; // read ":"
			cin >> val;
			md = convert_char(val);
			cin >> val;
			mu = convert_char(val);
			uint32_t time_x = ((uint32_t) hd << 21) + ((uint32_t) hu << 14) + ((uint32_t)md << 7) + ((uint32_t)mu << 0);
			all_recorded_times[j] = time_x;
			// ~time_x -> all zeros become 1 and 1s become 0
			// by doing &= , we can store which segments have always been "off"
			bit_mask |= time_x;
		}
		cout << "Case #" << i << ":" << endl;

		for (int time_i = 0; time_i < n_clcks; ++time_i){
			generate_and_test(all_recorded_times[time_i], 27, bit_mask, time_i);
		}

		// if (((~bit_mask) << 4) == 0 && test(all_recorded_times[0]))	// all segments work
		// 	gather_results.insert(make_pair(n_minutes_clock(all_recorded_times[0]), n_clcks));

		vector<int> minutes_valid = vector<int>();
		for (unordered_map<int,int>::iterator it = gather_results.begin(); it != gather_results.end(); it++){
			if (it->second == n_clcks )
				minutes_valid.push_back(it->first);
		}
		sort(minutes_valid.begin(), minutes_valid.end());
		if ( minutes_valid.size() == 0 )
			cout << "none" << endl;
		else {
			for ( int &c: minutes_valid)
				print_minutes_clock(c);
		}
	}

	return 0;
}