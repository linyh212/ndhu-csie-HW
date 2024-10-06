#include <iostream>
#include <cmath>
using namespace std;

class Segment{
	double x, y;
public:
	void setX(double _x){
    		x=_x;
	}
	void setY(double _y){
    		y=_y;
	}
	double getX(){
		return x;
	}
	double getY(){
		return y;
	}
	double distance(Segment length){
		double dx=x-length.getX();
		double dy=y-length.getY();
		return sqrt(dx*dx+dy*dy);
	}
};

int main(){
	int a;
	cin>>a;
	for(int i=0; i<a; i++){
		double length;
		Segment p1, p2;
		double x1, x2, y1, y2;
		cin>>x1>>y1>>x2>>y2;
		p1.setX(x1);
		p1.setY(y1);
		p2.setX(x2);
		p2.setY(y2);		
		cout<<p1.distance(p2)<<endl;
	}
}
