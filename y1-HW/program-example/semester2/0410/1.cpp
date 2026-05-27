#include <iostream>

using namespace std;

class B {
private:
  B() {
  }
  B(int s) {
  }
  int data;
  int number;
};

class D : private B {
};

int main() {
  D d;
  d.data = 10;
}