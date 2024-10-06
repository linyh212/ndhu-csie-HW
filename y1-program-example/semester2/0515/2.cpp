#include <iostream>
#include <fstream>

using namespace std;

int main()
{
  ifstream in;
  char fileName[1024];
  cin >> fileName;
  in.open(fileName);
  char buf;
  if(!in.good())
    cout << "fails to open file " << fileName << endl;
  while(in >> buf)
    cout << buf;
  in.close();
}