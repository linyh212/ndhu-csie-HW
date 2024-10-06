#include <iostream>
#include <cstdlib>
using namespace std;

class MyContainer {
  int *data;
  int _size;
  int capacity;
public:
  class iterator {
    int *ptr;
  public:
    iterator() : ptr(NULL) {}
    iterator(int *p) : ptr(p) {}
    int &operator*() {
      return *ptr;
    }
    bool operator!=(iterator b) {
      return ptr != b.ptr;
    }
    void operator++() {
      ptr ++;
    }
    void operator--() {
      ptr --;
    }
    iterator operator-(int n) {
      return iterator(ptr - n);
    }
    iterator operator+(int n) {
      return iterator(ptr + n);
    }
  };
  MyContainer() : data(NULL), _size(0), capacity(0) {}
  void resize(int s) {
    int *temp = (int *)malloc(sizeof(int) * s);
    int j;
    for(j = 0;j < _size;j ++)
      temp[j] = data[j];
    free(data);
    data = temp;
    capacity = s;
  }
  void push_back(int d) {
    if(capacity == 0)
      resize(2);
    if(_size == capacity)
      resize(capacity * 2);
    data[_size] = d;
    _size ++;
  }
  void insert(iterator p, int d) {
    if(_size == capacity)
      resize(capacity * 2);
    for(iterator it = end();it != p;-- it)
      *it = *(it - 1);
    *p = d;
    _size ++;
  }
  void erase(iterator p) {
    for(iterator it = p;it != end();++ it)
      *it = *(it + 1);
    _size --;
  }
  void erase(iterator p1, iterator p2) {
    while(p1 != p2) {
      erase(p1);
      -- p2;
    }
  }
  int size() {
    return _size;
  }
  int &operator[](int index) {
    return data[index];
  }
  iterator begin() {
    return iterator(data);
  }
  iterator end() {
    return iterator(data + _size);
  }
};

int main() {
  MyContainer mc;
  int j;
  for(j = 0;j < 10;j ++)
    mc.push_back(j + 1);
  mc.insert(mc.begin() + 1, 100);
  for(j = 0;j < mc.size();j ++)
    mc[j] ++;
  for(int &k : mc)
    k ++;
  for(MyContainer::iterator it = mc.begin();it != mc.end();++ it)
    cout << *it <<endl;
  cout << endl;
  mc.erase(mc.begin() + 1, mc.begin() + 5);
  for(MyContainer::iterator it = mc.begin();it != mc.end();++ it)
    cout << *it <<endl;
}


