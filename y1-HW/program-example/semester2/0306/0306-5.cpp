#include <iostream>
using namespace std;
class Triangle {
public:
  double area(double b, double h) {
    return b * h / 2.0;
  }
  double area(double a, double b, double c) {
    double s = (a + b + c) / 2.0;
    s = s * (s - a) * (s - b) * (s - c);
    double u = s, l = 0, m;
    while(u - l > 0.000001) {
      m = (u + l) / 2.0;
      if(m * m > s)
        u = m;
      else
        l = m;
    }
    return l;
  }
};

int main() {
  Triangle p;
  cout << p.area(3.0, 4.0) << endl;
  cout << p.area(3.0, 4.0, 5.0) << endl;
}