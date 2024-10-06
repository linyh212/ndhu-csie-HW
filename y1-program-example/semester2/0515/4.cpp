#include <iostream>
#include <fstream>

using namespace std;

int main() {
  fstream file;
  file.open("output2.txt", fstream::in);
  char str[1024];
  string s;
  //while(getline(file, s))
  //  cout << s;
  //cout << endl;
  while(file.getline(str, 1024))
    cout << str;
  file.close();
}