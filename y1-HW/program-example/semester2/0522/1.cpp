#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
  time_t start, end;
  int arr[100000];
  srand(time(NULL));
  for_each(arr, arr + 100000,[] (int &n) {n = rand();});
  //for_each(arr, arr + 100000,[](int n) {cout << n << " ";});
  int j, k;
  start = clock();
  do {
    k = 0;
    for(j = 1;j < 100000;j ++) {
      if(arr[j - 1] > arr[j]) {
        int temp = arr[j - 1];
        arr[j - 1] = arr[j];
        arr[j] = temp;
        k = 1;
      }
    }
  }while(k);
  end = clock();
  cout << end - start;
  //for_each(arr, arr + 100000,[](int n) {cout << n << " ";});
}