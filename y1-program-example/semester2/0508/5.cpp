#include <iostream>
using namespace std;

class Fraction {
  friend double operator+(const double a, const Fraction &b);
  int numerator, demoninater;
public:
  Fraction(int n, int d) : numerator(n), demoninater(d) {}
  double operator+(double b) {
    return static_cast<double>(numerator) / static_cast<double>(demoninater) + b;
  }
};

double operator+(const double a, const Fraction &b) {
  return a + static_cast<double>(b.numerator) / static_cast<double>(b.demoninater);
}

int main() {
  Fraction f(2, 3);
  double d = 1.5;
  cout << (d + f) << endl;
  cout << f + d << endl;
}