#include <iostream>
using namespace std;

class Segment{
public:
	bool point(double p1, double p2, double p3, double p4){
		return !(p3>p2||p4<p1);
	}
};

int main(){	
	int x, y, m, n;
	cin>>x>>y>>m>>n;
	double temp;
	if(x>y){
		temp=x;
		x=y;
		y=temp;
	}
	if(m>n){
		temp=m;
		m=n;
		n=temp;
	}
	Segment segment;
	bool a=segment.point(x, y, m, n);
	if(a) cout<<"overlay"<<endl;
	else cout<<"no overlay"<<endl;
}
