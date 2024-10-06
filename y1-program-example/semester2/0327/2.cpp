#include <iostream>

class B {
public:
  int x;
private:
  int y;
protected:
  int z;
};

class D : protected B {
public:
  void foo() {
    x = 10;
    //y = 10;
    z = 10;
  }
};

class DD : D {
public:
  void foo() {
    x = 10;
    //y = 10;
    z = 10;
  }
};

int main()
{
  //D d;
  //d.x = 10;
  //d.y = 10;
  //d.z = 10;
}