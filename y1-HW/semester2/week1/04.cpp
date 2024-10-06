#include <iostream>
using namespace std;

class Velocity{
public:
	double speed(double distance, double time){
		if(time==0) return 0;
		else return distance/time;
	}
};

int main(){
	double x, y;
	cin>>x>>y;
	Velocity v;
	cout<<v.speed(x, y)<<endl;
}
