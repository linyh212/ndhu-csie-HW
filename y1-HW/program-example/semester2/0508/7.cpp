#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>

using namespace std;

int main() {
  vector<int> v;
  for(int j = 0;j < 10;j ++)
    v.push_back(j + 1);
  /*for(int j = 0;j < 15;j ++) {
    try {
      cout << v.at(j) << endl;
    }
    catch(out_of_range e) {
      cout << e.what() << endl;
    }
  }*/
  try {
    for(int j = 0;j < 15;j ++)
      cout << v.at(j) << endl;
  }
  catch(out_of_range e) {
    cout << e.what() << endl;
  }
}