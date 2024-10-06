#include <iostream>
class Point {
  double x, y;
public:
  Point() : x(0), y(0) {}
  Point(double _x, double _y) : x(_x), y(_y) {}
  double getX() {return x;}
  double getY() {return y;}
  /*bool operator==(Point b) { //a == b a.operotar==(b)            
    if(x == b.x && y == b.y)
      return true;
    return false;
  }
  Point operator+(Point b) {
    return Point(x + b.x, y + b.y);
  }
  Point &operator++() {
    x ++;
    return *this;
  }
  Point &operator++(int ) {
    x ++;
    return *this;
  }*/
};

bool operator==(Point a, Point b) {
  if(a.getX() == b.getX() && a.getY() == b.getY())
    return true;
  return false;
}
Point operator+(const Point a, const Point b) {
  return Point(a.getX() + b.getX(), x.getY() + b.getY());
}
Point &operator++(Point a) {
}
Point &operator++(Point b, int ) {
}

int main()
{
  Point p1, p2(1, 1);
  if(p1 == p2) { // operator==(p1, p2);p1.operator(p2);
    std::cout << "same";
  }
  else {
    std::cout << "not";
  }
  Point c = p1 + p2;
  ++ p1; // p1.operator++()
  p1 ++; // p1.operator++(1)
}