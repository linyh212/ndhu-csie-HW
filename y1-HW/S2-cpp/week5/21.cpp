#include <bits/stdc++.h>
using namespace std;

class Time{
private:
    int hr, min, sec;
    void setTime(const string& timeStr){
        istringstream iss(timeStr);
        char colon;
        iss>>hr>>colon>>min>>colon>>sec;
    }
    int toSeconds() const{
        return hr*3600+min*60+sec;
    }
public:
    Time():hr(0), min(0), sec(0){}
    Time(const string& timeStr){
        setTime(timeStr);
    }
    string toString() const{
        stringstream ss;
        ss<<(hr<10? "0":"")<<hr<<":"<<(min<10? "0":"")<<min<<":"<<(sec<10? "0":"")<<sec;
        return ss.str();
    }
    int operator-(const Time& other) const{
        return abs(toSeconds()-other.toSeconds());
    }
};

int main(){
    string n, m;
    cin>>n>>m;
    Time t1(n), t2(m);
    cout<<t1.toString()<<endl;
    cout<<t2.toString()<<endl;
    cout<<(t1-t2)<<endl;
    return 0;
}
