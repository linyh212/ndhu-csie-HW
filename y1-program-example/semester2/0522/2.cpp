#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;

#define SIZE 100000

class Sorter {
  int *arr;
  void delFile(int n) {
    char command[1024];
    int j;
    for(j = 1;j <= n;j ++) {
      sprintf(command, "del %d.txt", j);
      system(command);
    }
  }
  void split(int n) {
    int j, k;
    int numberOfElement = SIZE / n;
    if(SIZE % n != 0)
      numberOfElement ++;
    char fileName[1024];
    for(j = 1, k = 0;j <= n;j ++) {
      sprintf(fileName, "%d.txt", j);
      fstream out(fileName, fstream::out);
      do {
        out << arr[k] << " ";
        k ++;
      }while(k < SIZE && k % numberOfElement != 0);
      out.close();
    }
  }
public:
  Sorter(int *a) : arr(a) {}
  friend ostream &operator<<(ostream &out, Sorter s) ;
  void sort(int n) {
    split(n);
    char fileName[1024];
    int array[SIZE];
    int i, j, k, l, index = 0;
    int buf[SIZE], a, b, c;
    for(i = 1;i <= n;i ++) {
      sprintf(fileName, "%d.txt", i);
      fstream in(fileName, fstream::in);
      j  = 0;
      while(in >> k)
        array[j ++] = k;
      do {
        k = 0;
        for(l = 1;l < j;l ++) {
          if(array[l - 1] > array[l]) {
            int temp = array[l - 1];
            array[l - 1] = array[l];
            array[l] = temp;
            k = 1;
          }
        }
      }while(k);
      a = b = c = 0;
      while(a < j && b < index) {
        if(array[a] < arr[b])
          buf[c ++] = array[a ++];
        else
          buf[c ++] = arr[b ++];
      }
      while(a < j)
        buf[c ++] = array[a ++];
      while(b < index)
        buf[c ++] = arr[b ++];
      for(k = 0;k < c;k ++)
        arr[k] = buf[k];
      index = c;
    }
    delFile(n);
  }
};
ostream &operator<<(ostream &out, Sorter s) {
    int j;
    for(j = 0;j < SIZE;j ++)
      out << s.arr[j] << " ";
    out << endl;
    return out;
  }

int main() {
  time_t start, end;
  int arr[SIZE];
  srand(time(NULL));
  for_each(arr, arr + SIZE,[] (int &n) {n = rand();});
  Sorter s(arr);
  start = clock();
  s.sort(20);
  end = clock();
  cout << end - start;
  //cout << s;
}