#include <algorithm>    // nth_element
#include <array>        // array
#include <chrono>       // high_resolution_clock
#include <iomanip>      // setw
#include <iostream>     // cout
#include <vector>       // vector

using namespace std::chrono;
using namespace std;

const unsigned MAXLINE = 1024; // maximun line size to test
const unsigned GAP = 12;       // gap for cout columns
const unsigned REP = 100;      // number of repetitions of every test

int main(){
	cout << "#" << setw(GAP - 1) << "line (B)" << setw(GAP) << "time (µs)" << endl;
	
	for (unsigned line = 1; line <= MAXLINE; line <<= 1){ // line in bytes
		
		vector<duration<double, micro>> score(REP);
		
		for (auto &s: score){
			vector<char> bytes(1 << 24); // 16MB
			
			auto start = high_resolution_clock::now();
			
			for (unsigned i = 0; i < bytes.size(); i += line)
				bytes[i] ^= 1;
			
			auto stop = high_resolution_clock::now();
			
			s = stop - start;
		}
		
		nth_element(score.begin(), score.begin() + score.size() / 2, score.end());
		
		cout << setw(GAP) << line << setw(GAP) << fixed << setprecision(1) << setw(GAP) << score[score.size() / 2].count() << endl;
	}
}

