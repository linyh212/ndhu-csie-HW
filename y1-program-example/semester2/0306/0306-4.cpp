#include<iostream>

using namespace std;

class Foo {
private:
  int intVar;
  int func1() {
    return 1;
  }
public:
  float fVar;
  int func2() {
    return 1;
  }
  int func3() {
    return func1();
  }
};

int main()
{
  int i;
  Foo *p = (Foo *)(&i);
  /*Foo foo;
  foo.func3();
  p = &foo;
  (*p).fVar = 100;
  p->fVar = 1.234;
  p = new Foo;*/
  delete p;
  p->fVar = 100;
  return 0;
}