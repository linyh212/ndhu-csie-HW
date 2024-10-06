#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

class Foo {
  double *data;
  string name;
public:
  void setName(string n)
  {
    name = n;
  }
  Foo() {
    *this = Foo("no_name");
    name = "no_name";
  }
  void askData() {
    data = (double *)malloc(sizeof(double) * 100);
  }
  Foo(string n) : name(n) {
    //name = n;
    cout << name << " constructor" << endl;
  }
  ~Foo() {
    cout << name << " destructor" << endl;
  }
  string getName() {
    return name;
  }
};

int main()
{
  /*Foo foo("object_1");
  foo.~Foo();*/
  /*Foo *p = new Foo("object_1");
  cout << p->getName() << endl;
  delete p;*/
  /*Foo *p = new Foo;
  //Foo *p = (Foo *)malloc(sizeof(Foo));
  //p->setName("o2");
  cout << p->getName() << endl;*/
  Foo foo;
  foo.askData();
}





