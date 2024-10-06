#include <iostream>
#include <vector>
using namespace std;

int main()
{
  vector<int> c;
  int j;
  for(j = 0;j < 10;j ++)
    c.push_back(j);
  //for(j = 0;j < 10;j ++)
  //  c[j] = c[j] + 1;
  //c.insert(c.begin() + 5, 10);
  //for(j = 0;j < 10;j ++)
  //  cout << c[j] << endl;
  //vector<int>::iterator it;
  //for(it = c.begin();it != c.end();it ++)
  //  cout << *it << endl;
  c.pop_back();
  c.erase(c.begin() + 4, c.begin() + 6);
  for(int &k : c) {
    k ++;
  }
  for(int k : c) {
    cout << k << endl;
  }
  
  //cout << c.size() << endl;
}