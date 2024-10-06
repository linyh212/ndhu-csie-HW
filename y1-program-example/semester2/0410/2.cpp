#include <iostream>
#include <string>

using namespace std;

class B {
public:
  virtual void foo() {
    cout << "B" << endl;
  }
  virtual ~B() {
    cout << "destory B" << endl;
  }
};

class D : public B {
public:
  void foo() {
    cout << "D" << endl;
  }
  ~D() {
    cout << "destory D" << endl;
  }
};

class D2 : public B {
public:
  void foo() {
    cout << "D2" << endl;
  }
  ~D2() {
    cout << "destory D2" << endl;
  }
};

class D3 : public B {
public:
  void foo() {
    cout << "D3" << endl;
  }
  ~D3() {
    cout << "destory D3" << endl;
  }
};

int main() {
  int j;
  D *d[10];
  D2 *d2[10];
  D3 *d3[10];
  for(j = 0;j < 10;j ++) {
    d[j] = new D;
    d2[j] = new D2;
    d3[j] = new D3;
  }
  //for(j = 0;j < 10;j ++) {
  //  d3[j]->foo();
  //}
  B *b[30];
  for(j = 0;j < 10;j ++)
  {
    b[j] = d[j];
    b[j + 10] = d2[j];
    b[j + 20] = d3[j];
  }
  for(j = 0;j < 30;j ++)
    b[j]->foo();
  for(j = 0;j < 30;j ++)
    delete b[j];
}