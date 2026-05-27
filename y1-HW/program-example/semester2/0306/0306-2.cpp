#include <stdio.h>
class A {
public:
  void read() {
    scanf("%c", &ch);
  }
  int get() {
    return ch;
  }
private:
  char ch;
};
class B {
public:
  void put(char ch) {
    printf("%d", ch);
  }
};
class C {
public:
  void put(char ch) {
    printf("%c", ch);
  }
};

int main() {
  A a, aa;
  B b;
  C c;
  a.read();
  a.ch = 'A';
  b.put(a.get());
  c.put(a.get());
}