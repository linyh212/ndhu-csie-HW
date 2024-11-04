#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <chrono>
#include <numeric>
using namespace std;

void Magic(const int n){
    const int MaxSize=51;
    int square[MaxSize][MaxSize], k, l;
    if((n>MaxSize)||(n<1))	throw "Error!..n out of range!";
    else if(n%2==0) throw "Error!..n is even!";
    for(int i=0; i<n; i++)
		fill(square[i], square[i]+n, 0);// Using STL to initialize
    square[0][(n-1)/2]=1;
    int key=2, i=0, j=(n-1)/2;
    while(key<=n*n){
		if(i-1<0) k=n-1;
		else k=i-1;
		if(j-1<0) l=n-1;
		else l=j-1;
		if(square[k][l]) i=(i+1)%n;
		else{
		    i=k;
		    j=l;
		}
		square[i][j]=key;
		key++;
    }
}

int main(){
	const vector<int> sizes={11, 21, 31, 41, 51};
	const vector<int> iterations={10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
	for(int size: sizes){
		cout<<"Testing size "<<size<<":"<<endl;
		for(int itr: iterations){
			vector<long long> t;
			for(int i=0; i<itr; i++){
				auto start=chrono::high_resolution_clock::now();
				Magic(size);
				auto end=chrono::high_resolution_clock::now();
				t.push_back(chrono::duration_cast<chrono::nanoseconds>(end-start).count());
			}
			long long total=accumulate(t.begin(), t.end(), 0LL);
			double avg=static_cast<double>(total)/itr;
			cout<<"Iterations: "<<itr<<", Total time(nanoseconds): "<<total<<", Average time(nanoseconds): "<<avg<<endl;
		}
		cout<<"----------------------------------------\n";
	}
    return 0;
}
