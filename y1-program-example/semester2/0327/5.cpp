#include <iostream>
using std::cout;
using std::endl;
class A {
public:
  void fun() {
    cout << "This is parent class" << endl;
  }
};

class B : public A {
public:
  void fun2() {
    cout << "This is child class" << endl;
  }
};

int main() {
  A a;
  B b;
  a.fun();
  b.fun2();
  b.fun();
}













