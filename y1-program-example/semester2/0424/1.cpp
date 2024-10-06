#include <iostream>

using namespace  std;

class MyContainer {
  friend ostream &operator<<(ostream &out, const MyContainer &mc);
  char *data;
  int capacity;
  int index;
public:
  class iterator {
    char *ptr;
  public:
    iterator() : ptr(nullptr) {}
    iterator(char *p) : ptr(p) {}
    bool operator!=(const iterator &o) const {
      return ptr != o.ptr;
    }
    char operator*() {
      return *ptr;
    }
    void operator++() {
      ptr ++;
    }
  };
  MyContainer() : data(nullptr), capacity(0), index(0) {}
  void resize() {
    if(capacity == 0) {
      data = new char[2];
      capacity = 2;
    }
    else {
      char *temp = new char[capacity * 2];
      for(int j = 0;j < capacity;j ++)
        temp[j] = data[j];
      delete data;
      data = temp;
      capacity *= 2;
    }
  }
  void push_back(char d) {
    if(index == capacity)
      resize();
    data[index] = d;
    index ++;
  }
  char at(int i) const {
    return data[i];
  }
  char &operator[](int i) {
    return data[i];
  }
  iterator begin() {
    return iterator(data);
  }
  iterator end() {
    return iterator(data + index);
  }
  void operator+(const string &str) {
    for(auto j = str.begin();j != str.end(); j ++)
      push_back(*j);
  }
};

void operator+(const string &str, MyContainer &mc) {
  for(auto j = str.begin();j != str.end();j ++)
    mc.push_back(*j);
}

ostream &operator<<(ostream &out, const MyContainer &mc) {
  for(int j = 0;j < mc.index;j ++)
    out << mc.at(j);
  return out;
}

void operator>>(istream &in, MyContainer &mc) {
  string temp;
  cin >> temp;
  for(char j : temp)
    mc.push_back(j);
}

int main()
{
  MyContainer mc;
  for(int j = 0;j < 10;j ++)
    mc.push_back('A' + j);
  for(int j = 0;j < 10;j ++)
    cout << mc.at(j);
  cout << endl;
  for(int j = 0;j < 10;j ++)
    mc[j] ++;
  for(int j = 0;j < 10;j ++)
    cout << mc.at(j);
  cout << endl;
  for(auto j : mc)
    cout << j;
  cout << endl;
  mc + "XYZ"; // mc.operaotr+("XYZ");
  for(auto j : mc)
    cout << j;
  cout << endl;
  "xyz" + mc; // "XYZ".operator+(mc); or operator+(string, MyContainer);
  for(auto j : mc)
    cout << j;
  cout << endl;
  cout << mc << endl;
  cout << endl;
  cin >> mc;
  cout << mc;
}

