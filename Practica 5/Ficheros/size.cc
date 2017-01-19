#include <algorithm>    // nth_element
#include <array>        // array
#include <chrono>       // high_resolution_clock
#include <iomanip>      // setw
#include <iostream>     // cout
#include <vector>       // vector

using namespace std::chrono;
using namespace std;

const unsigned MINSIZE = 1 << 10; // minimun line size to test:  1KB
const unsigned MAXSIZE = 1 << 26; // maximun line size to test: 32MB
const unsigned GAP = 12;          // gap for cout columns
const unsigned REP = 100;         // number of repetitions of every test
const unsigned STEPS = 1e6;       // steps

int main(){
	cout << "#"  << setw(GAP - 1) << "line (B)" << setw(GAP) << "time (µs)" << endl;
	
	for (unsigned size = MINSIZE; size <= MAXSIZE; size *= 2){
		vector<duration<double, micro>> score(REP);
		
		for (auto &s: score){
			vector<char> bytes(size);
			
			auto start = high_resolution_clock::now();
			
			for (unsigned j = 0; j < STEPS; ++j)
				bytes[(j * 64) & (size - 1)]++;		
				
				auto stop = high_resolution_clock::now();
			
			s = stop - start;
		}
		
		nth_element(score.begin(), score.begin() + score.size() / 2, score.end());
		
		cout << setw(GAP) << size << setw(GAP) << fixed << setprecision(1) << setw(GAP) << score[score.size() / 2].count() << endl;
	}
}
