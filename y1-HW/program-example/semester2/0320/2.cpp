#include <iostream>
#include <string>
using namespace std;

void foo(int &var) {
  var ++;
}

int main() {
  int intVar = 10;
  foo(100);
  cout << intVar << endl;
}

/*class Foo {
private:
  int data;
public:
  //void setData(int d) {data = d;}
  //int getData() {return data;}
  int &getData() {return data;}
};

int main()
{
  //Foo foo;
  //foo.setData(10);
  //cout << foo.getData() << endl;
  //Foo foo;
  //foo.getData() = 10;
  //cout << foo.getData() << endl;
}*/