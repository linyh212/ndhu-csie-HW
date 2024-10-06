#include <iostream>
#include <string>
using namespace std;

class Foo {
private:
  int data = 0;
public:
  void fun1() const {
    data ++;
  }
};

int main()
{
  Foo foo;
  foo.fun1();
}






