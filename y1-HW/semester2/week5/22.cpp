#include <bits/stdc++.h>
using namespace std;

class Date{
private:
    int yr, mon, day;
    void setDate(const string& dateStr){
        istringstream iss(dateStr);
        char c;
        iss>>yr>>c>>mon>>c>>day;
    }
    int toDays() const{
    	int total=0;
    	total+=yr*365+(yr-1)/4-(yr-1)/100+(yr-1)/400;
    	static const int daysInMonth[]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        for(int i=1; i<mon; i++){
            total+=daysInMonth[i];
        }
        total+=day;
        if(mon>2&&((yr%4==0&&yr%100!=0)||yr%400==0)){
            total++;
        }
        return total;
    }
public:
    Date():yr(1900), mon(1), day(1){}
    Date(const string& dateStr){
        setDate(dateStr);
    }
    string toString() const{
        stringstream ss;
        ss<<(yr<10? "0":"")<<yr<<"/"<<(mon<10? "0":"")<<mon<<"/"<<(day<10? "0":"")<<day;
        return ss.str();
    }
    int operator-(const Date& other) const{
        return abs(toDays()-other.toDays());
    }
};

int main(){
    string n, m;
    cin>>n>>m;
    Date d1(n), d2(m);
    cout<<d1.toString()<<endl;
    cout<<d2.toString()<<endl;
    cout<<(d1-d2)<<endl;
    return 0;
}
