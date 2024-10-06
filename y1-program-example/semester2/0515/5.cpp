#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

int main() {
  char fileName[1024];
  cin >> fileName;
  filesystem::path myFilePath(fileName);
  if(filesystem::exists(myFilePath))
    cout << "exist" << endl;
  else
    cout << "not exist" << endl;
}