#include <iostream>

using namespace std;

class B {
public:
  void foo() {
    cout << "foo in B" << endl;
  }
};

class D : public B {
public:
  void foo() {
    cout << "foo in D" << endl;
  }
};

  int a = 10;

int main() {
  //B b;
  //b.foo();
  //D d;
  //d.B::foo();
  //int a = 100;
  //cout << ::a << endl;
  D *b = (D *)new B();
  b->foo();
  //B *b = new D();
  //b->foo();
}
