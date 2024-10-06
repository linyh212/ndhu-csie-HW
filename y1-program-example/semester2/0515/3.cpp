#include <iostream>
#include <fstream>

using namespace std;

int main() {
  fstream file;
  file.open("output2.txt", fstream::app);
  file << "some content......" << endl;
  file.close();
}