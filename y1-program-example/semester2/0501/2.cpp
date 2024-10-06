#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

bool compare(int a, int b) {
  return a > b;
}

int main() {
  int arr[10];
  srand(time(NULL));
  //for(int j = 0;j < 10;j ++)
  //  arr[j] = rand();
  for_each(arr, arr + 10, [](int &j){j = rand();});
  for(auto j : arr)
    cout << j << " ";
  cout << endl;
  //lambda
  //sort(arr, arr+ 10, compare);
  sort(arr, arr + 10, [] (int a, int b) ->bool {return a > b;} );
  //for(auto j : arr)
  //  cout << j << " ";
  for_each(arr, arr + 10, [] (int j) {cout << j << " ";});
  cout << endl;
}