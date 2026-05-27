#include <iostream>
using namespace std;
class B {
public:
  virtual ~B() {
    cout << "destory B" << endl;
  }
};

class  D : public B {
public:
  ~D() {
    cout << "destory D" << endl;
  }
};

int main()
{
  B *b;
  b = new D;
  delete b;
}