#include <iostream>
#include <cstdio>
using namespace std;

int main()
{
  /*const int a = 10;
  cout << a << endl;
  a = 100;
  cout << a << endl;*/
  int b = 10, c = 100;
  /* int const *  ptr = &b;
  cout << *ptr << endl;
  //*ptr = 100;
  ptr = &c;
  cout << *ptr << endl;
  cout << b << endl;*/
  /*int d = 100, e = 10;
  const int * const p = &d;
  *p = 1;
  cout << d << endl;
  p = &e;*/

  const int var = 1;
  int *pointer = (int *)&var;
  int **ptr = (int **)&pointer;
  ptr ++;
  (*ptr) ++;
  (**ptr) ++;
  
}