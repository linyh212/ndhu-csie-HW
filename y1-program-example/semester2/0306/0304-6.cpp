#include <iostream>
using namespace std;
class Triangle {
public:
  Triangle() {
    cout << "Tringle constructed" <<endl;
    sideA = sideB = sideC = 0;
  }
  Triangle(double a, double b, double c) {
    sideA = a;
    sideB = b;
    sideC = c;
    cout << "Tringle constructed" <<endl;
    cout << "(" << a << ", " << b << ", " << c << ")" << endl;
  }
  double sideA, sideB, sideC;
  ~Triangle() {
    cout << "destructor" << endl;
    cout << "(" << sideA << ", " << sideB << ", " << sideC << ")" << endl;
  }
};

int main() {
  //{
    Triangle t(3, 4, 5);
  //}
  Triangle tt;
}