#include <bits/stdc++.h>
using namespace std;

class Calculate{
public:
    Calculate(int start, int end): start_(start), end_(end){}
    int calculateMaxCycleLength(){
        int max=0;        
        for(int i=start_;i<=end_; i++){
            int cycleLength=getCycleLength(i);
            if(cycleLength>max) max=cycleLength;
        }
		return max;
    }
private:
    int getCycleLength(int n){
        int length=1;
		while(n!=1){
			if(n%2!=0) n=3*n+1;
            else n=n/2;
			length++;
        }        
        return length;
    }
    int start_;
    int end_;
};

int main(){
    int i, j;
    while(cin>>i>>j){
    	cout<<i<<" "<<j<<" ";
    	if(i>j) swap(i, j);
        Calculate cal(i, j);
        int l=cal.calculateMaxCycleLength();
        cout<<l<<endl;
	}
	return 0;
}
