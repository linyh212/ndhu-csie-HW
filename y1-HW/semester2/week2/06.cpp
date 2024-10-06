#include <iostream>
using namespace std;

class Point{
	float x, y;
public:
	void setX(float _x){
		x=_x;
	}
	void setY(float _y){
		y=_y;
	}
	float getX(){
		return x;
	}
	float getY(){
		return y;
	}
};

int main(){
	int a;
	cin>>a;
	for(int i=0; i<a; i++){
		Point point;
		float x, y;
		cin>>x>>y;
		point.setX(x);
		point.setY(y);
		cout<<"("<<point.getX()<<", "<<point.getY()<<")"<<endl;
	}
}
