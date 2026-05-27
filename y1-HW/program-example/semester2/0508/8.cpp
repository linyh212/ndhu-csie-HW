#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>

using namespace std;

class Foo {
  vector<string> &db;
public:
  Foo(vector<string> &d) : db(d) {}
  void insert(string data) {
    if(data.size() > 10)
      throw length_error("too long...");
    for_each(db.begin(), db.end(), [data] (string n) {
	    if(data == n)
	        throw string("data already exist.");
	});
    db.push_back(data);
  }
};

int main() {
  vector<string> db;
  Foo foo(db);
  while(1) {
    string str;
    cin >> str;
    if(str == "exit")
      break;
    try {
     foo.insert(str);
    }
    catch(string e) {
      cout << e << endl;
    }
    catch(length_error e) {
      cout << e.what() << endl;
    }
    
  }
  for_each(db.begin(), db.end(), [] (auto n) {cout << n << endl;});
}
