#include <iostream>
#include <cmath>
using namespace std;

class Tollway{
public:
	float amount;
	int money(int distance){
		if(distance<=200) amount=distance*1.2;
		else amount=200*1.2+(distance-200)*1.2*0.75;
		return round(amount);
	}
};

int main(){
	int x;
	cin>>x;
	Tollway tollway;
	cout<<tollway.money(x)<<endl;
}
