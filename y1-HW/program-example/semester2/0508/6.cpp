#include <iostream>
using namespace std;

class Fraction {
  int numerator, demoninater;
public:
  Fraction(int n, int d) : numerator(n), demoninater(d) {}
  operator double() {
    cout << "!!!!!!!!!!" ;
    return static_cast<double>(numerator) / static_cast<double>(demoninater);
  }
};

int main() {
  Fraction f(2, 3);
  double d = 1.2;
  /*cout << d + f << endl;
  cout << f + d << endl;
  cout << f << endl;
  int i = 10;
  cout << f + i << endl;*/
  //(double)f;
  static_cast<double>(f);
}