#include <fstream>
#include <iostream>
using namespace std;

int main() {
  ofstream out;
  out.open("output1.txt");
  out << "something!!!!" << endl;
  out.close();
}