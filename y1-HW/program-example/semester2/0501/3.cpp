#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
  [] () {};
  int arr[10];
  int j = 123;
  int n = 100;
  for_each(arr, arr + 10, [&n] (int &j) {j = n; n ++;j = n;});
  for(auto j : arr)
    cout << j << " ";
  cout << endl;
  cout << n;
}