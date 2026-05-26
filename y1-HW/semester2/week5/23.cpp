#include <iostream>
#include <sstream>
using namespace std;

class Time{
private:
    int hr, min, sec;
public:
    Time():hr(0), min(0), sec(0){}
    Time(string timeStr){
        stringstream ss(timeStr);
        char c;
        ss>>hr>>c>>min>>c>>sec;
    }
    string toString() const{
        stringstream ss;
        ss<<(hr<10? "0":"")<<hr<<":"<<(min<10? "0":"")<<min<<":"<<(sec<10? "0":"")<<sec;
        return ss.str();
    }
};

class Date{
private:
    int yr, mon, day;
public:
    Date():yr(1900), mon(1), day(1){}
    Date(string dateStr) {
        stringstream ss(dateStr);
        char c;
        ss>>yr>>c>>mon>>c>>day;
    }
    string toString() const{
        stringstream ss;
        ss<<yr<<"/"<<(mon<10?"0":"")<<mon<<"/"<<(day<10? "0":"")<<day;
        return ss.str();
    }
};

class DateTime{
private:
    Date date;
    Time time;
public:
    DateTime():date(), time(){}
    DateTime(const string& dtStr):date(dtStr.substr(0, 10)), time(dtStr.substr(11)){}
    string toString() const{
        return date.toString()+" "+time.toString();
    }
};

int main(){
    string n, m;
    getline(cin, n);
    getline(cin, m);
    DateTime dt1(n), dt2(m);
    cout<<dt1.toString()<<endl;
    cout<<dt2.toString()<<endl;
}

