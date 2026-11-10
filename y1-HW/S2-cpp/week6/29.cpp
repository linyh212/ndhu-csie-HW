#include <bits/stdc++.h>
using namespace std;

class Point{
private:
    double x, y;
public:
    Point(): x(0), y(0){}
    Point(double a, double b): x(a), y(b){}
    void printPoint() const{
        cout<<"("<<x<<", "<<y<<")";
    }
    double getX() const{return x;}
    void setX(double c){x=c;}
    double getY() const{return y;}
    void setY(double d){y=d;}
    double getDistance(Point e){
        return sqrt((x-e.getX())*(x-e.getX())+(y-e.getY())*(y-e.getY()));
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
    Point getStartPoint() const{return point1;}
    Point getEndPoint() const{return point2;}
};
class Shape{
public:
    Shape(): edgeCount(0){}
    Shape(Segment *s, int count): edgeCount(count){ // to construct a shape with a serial of segment. Throw an invalid_argument if the segment can not construct a close path.
        if(count<3||count>10) throw invalid_argument("Cannot construct a close path.");
        for(int i=0; i<count; i++){
            seg[i]=s[i];
        }
        if(count>1&&(seg[0].getStartPoint().getX()!=seg[count-1].getEndPoint().getX()||seg[0].getStartPoint().getY() != seg[count-1].getEndPoint().getY()))
			throw invalid_argument("Cannot construct a close path.");
	}
    virtual float getArea()=0;
    float getPerimeter(){
		float per=0;
		for(int i=0; i<edgeCount; i++){
			per+=seg[i].getLength();
		}
		return per;
    }
    Segment &getSeg(int index){
		return seg[index];
	}
protected:
    Segment seg[10];
    int edgeCount;
};
