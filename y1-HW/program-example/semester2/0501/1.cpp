#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class Point2D {
public:
  double x, y;
};

istream &operator>>(istream &in, Point2D &p) {
  in >> p.x >> p.y;
  return in;
}

ostream &operator<<(ostream &out, Point2D p) {
  out << "(" << p.x << ", " << p.y << ")";
  return out;
}

template<class T>
class Node {
  T data;
  Node *next;
public:
  Node(T d) : data(d), next(NULL) {}
  T &getData() {
    return data;
  }
  Node *&getNext() {
    return next;
  }
};

template<class T>
ostream &operator<<(ostream &out, Node<T> *j) {
  cout << j->getData();
  return out;
}

template<class T>
class MyContainer {
  friend ostream &operator<<(ostream &out, const MyContainer *mc) {
    Node<T> *j = mc->first;
    while(j != NULL) {
      out << j << " ";
      j = j ->getNext();
    }
    return out;
  }
  Node<T> *first;
public:
  class iterator {
    Node<T> *it;
  public:
    iterator(Node<T> *p) : it(p) {}
    bool operator!=(iterator i) {
      return it != i.it;
    }
    iterator &operator++() {
      it = it->getNext();
      return *this;
    }
    T &operator*() {
      return it->getData();
    }
  };
  MyContainer() : first(NULL) {}
  void push_back(T d) {
    Node<T> *newNode = new Node<T>(d);
    if(first == NULL)
      first = newNode;
    else {
      Node<T> *j = first;
      while(j->getNext() != NULL)
        j = j->getNext();
      j->getNext() = newNode;
    }
  }
  iterator begin() {
    return iterator(first);
  }
  iterator end() {
    return iterator(NULL);
  }
};

int main() {
  MyContainer<Point2D> *mc = new MyContainer<Point2D>();
  while(1) {
    string com;
    cin >> com;
    if(com == "insert") {
      Point2D buf;
      cin >> buf;
      mc->push_back(buf);
    }
    else if(com == "show") {
      //cout << mc << endl;
      //for(auto j : *mc) {
      //  cout << j << " ";
      //}
      //cout << endl;
      for_each(mc->begin(), mc->end(),  [] (Point2D j) ->void {cout << j << " ";});
      cout << endl;
    }
    else if(com == "exit") {
      break;
    }
  }
}