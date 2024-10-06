#include <bits/stdc++.h>
using namespace std;

class Point{
private:
    double x, y;
public:
    Point(): x(0), y(0){}
    Point(double v1, double v2): x(v1), y(v2){}
    void printPoint() const{
        cout<<"("<<x<<", "<<y<<")";
    }
    double getX() const{return x;}
    void setX(double a) {x=a;}
    double getY() const {return y;}
    void setY(double b) {y=b;}
    double getDistance(Point p) {
        return sqrt((x-p.getX())*(x-p.getX())+(y-p.getY())*(y-p.getY()));
    }
};

class Segment{
private:
    Point point1, point2;
public:
    Segment(): point1(Point()), point2(Point()){}
    Segment(Point p1, Point p2): point1(p1), point2(p2){}
    double getLength(){
        return point1.getDistance(point2);
    }
};

int main(){
    Point p1(0, 0);
    Point p2(1, 1);
    Segment s(p1, p2);
    p1.printPoint();
    p2.printPoint();
	cout<<s.getLength()<<endl;
	return 0;
}

