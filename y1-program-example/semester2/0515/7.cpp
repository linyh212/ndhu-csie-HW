#include <iostream>
#include <fstream>
using namespace std;
int main() {
  ifstream in("output2.txt", ifstream::binary);
  char buf;
  while(1) {
    buf = in.get();
    if(in.eof())
      break;
    cout << buf;
  }
  in.close();
}