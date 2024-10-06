#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

class Record {
  friend ostream &operator<<(ostream &out, const Record &r) {
    for_each(r.data.begin(), r.data.end(), [] (pair<string, string> n) {cout << n.first << ": " << n.second << endl;});
    return out;
  }
  map<string, string > data;
public:
  Record() {}
  void insert(string key, string value) {
    data.insert(pair<string ,string >(key, value));
  }
  string getValue(string key) {
    return data[key];
  }
};

class DataFrame {
  vector<Record> data;
  vector<string> colName;
public:
  DataFrame(string file) {
    fstream in(file, fstream::in);
    string buf;
    getline(in, buf);
    size_t start = 0, end;
    end = buf.find(",");
    while(end != -1) {
      string temp = buf.substr(start, end - start);
      colName.push_back(temp);
      start = end + 1;
      end = buf.find(",", start);
    }
    //for_each(colName.begin(), colName.end(), [] (string n) {cout << n << endl;});
    while(getline(in, buf)) {
      Record r;
      int j = 0;
      start = 0;
      end = buf.find(",");
      while(end != -1) {
        string temp = buf.substr(start, end - start);
        r.insert(colName[j], temp);
        j ++;
        start = end + 1;
        end = buf.find(",", start);
      }
      //cout << r << endl;
      data.push_back(r);
    }
  }
  Record lookup(string date) {
    Record r = *find_if(data.begin(), data.end(), [date] (Record r) {if(r.getValue("date") == date) return true; return false;});
    return r;
  }
  void del(string date) {
    vector<Record>::iterator it = find_if(data.begin(), data.end(), [date] (Record r) {if(r.getValue("date") == date) return true; return false;});
    data.erase(it);
  }
  void save(string file) {
    fstream out(file, fstream::out);
    for_each(data.begin(), data.end(), [&] (Record r) {
      for_each(colName.begin(), colName.end(), [&] (string n) {
        out << r.getValue(n) << ",";
      });
    });
    out.close();
  }
};

int main() {
  DataFrame df("russia_losses_equipment.csv");
  while(1) {
    string com;
    cin >> com;
    if(com == "exit")
      break;
    else if(com == "del") {
      cin >> com;
      df.del(com);
    }
    else if(com == "save") {
      cin >> com;
      df.save(com);
    }
    else {
      Record r = df.lookup(com);
      cout << r << endl;
    }
  }
}