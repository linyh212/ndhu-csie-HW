#include <iostream>
using namespace std;
class R {
  int width, length;
public:
  R(int w, int l) : width(w), length(l) {}
  int area() {
    return length * width;
  }
  int p() {
    return 2 * (length + width);
  }
};
class S : public R {
public:
  S(int s) : R(s, s) {}
};

int main() {
  R r(2, 5);

  cout << r.area() << endl;
  cout << r.p() << endl;

  S s(5);

  cout << s.area() << endl;
  cout << s.p() << endl;
  
}