#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

void Magic(const int n){
    const int MaxSize=51;
    int square[MaxSize][MaxSize], k, l;
    if((n>=MaxSize)||(n<1))	throw "Error!..n out of range!";
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
    cout<<"magic square of size "<<n<<endl;
    for(i=0 ; i<n; i++){
		copy(square[i], square[i]+n,ostream_iterator<int>(cout," "));
		cout<<endl;
    }
}

int main(){
    Magic(5);
    return 0;
}
