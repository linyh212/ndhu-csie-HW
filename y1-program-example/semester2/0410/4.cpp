#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

class Shape {
public:
  Shape(string n) : name(n) {}
  virtual double area() = 0;
  string name;
  string getName() {return name;}
};

class Triangle : public Shape {
  double height, botton;
public:
  Triangle(double h, double b) : height(h), botton(b), Shape("Triangle") {}
  double area() {
    return height * botton * 0.5;
  }
};

class Square : public Shape {
  double side;
public:
  Square(double s) : side(s), Shape("Square") {}
  double area() {
    return side *side;
  }
};

int main() {
  int j;
  Shape *s[10];
  srand(time(NULL));
  for(j = 0;j < 10;j ++)
    if(j % 2)
      s[j] = new Triangle(rand() % 10, rand() % 10);
    else
      s[j] = new Square(rand() % 10);
  for(j = 0;j < 10;j ++)
    cout << s[j]->getName() << ":" << s[j]->area() << endl;
}