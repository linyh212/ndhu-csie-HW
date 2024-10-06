#include <iostream>
using namespace std;
class Foo {
public:
  int dataInFoo;
  Foo(int d = 0) : dataInFoo(d) {}
};

class Bar : public Foo {
public:
  int dataInBar;
  Bar(int d1 = 0, int d2 = 0) : Foo(d1), dataInBar(d2) {}
};

int main() {
  Foo *f = new Bar(1, 4);
  Bar *b = new Bar(2, 3);
  cout << static_cast<Bar *>(f)->dataInFoo << ", " << static_cast<Bar *>(f)->dataInBar << endl;
  //cout << static_cast<Foo *>(b)->dataInFoo << endl;
  //cout << dynamic_cast<Foo *>(b)->dataInFoo << endl;
}