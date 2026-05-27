#include <iostream>

using namespace std;

class B {
public:
  B() {
    cout << "default constructor in class B" << endl;
  }
  B(int ) {
    cout << "constructor in class B with parameter" << endl;
  }
  ~B() {
    cout << "destructor in class B" << endl;
  }
};

class D : public B {
public:
  D() : B(123) {
    cout << "default constructor in class D" << endl;
  }
  ~D() {
    cout << "destructor in class D" << endl;
  }
};

int main()
{
  //B b(123);
  D d;
}