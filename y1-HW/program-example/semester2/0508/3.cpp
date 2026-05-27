class Foo {
  int dataInFoo;
public:
  Foo(int d = 0) : dataInFoo(d) {}
};

class Bar : public Foo {
  int dataInBar;
public:
  Bar(int d1 = 0, int d2 = 0) : Foo(d1), dataInBar(d2) {}
};

int main() {
  Foo *f = new Foo(1);
  Bar *b = new Bar(2, 3);
  cout << static_cast<Bar *>(f)->dataInFoo << ", " << static_cast<Bar *>(f)->dataInBar;
}