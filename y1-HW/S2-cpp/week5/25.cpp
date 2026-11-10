#include <bits/stdc++.h>
using namespace std;

class GridCity{
private:
    vector<int> streets, avenues;
public:
    GridCity(int s, int a){}
    void add_Location(int street, int avenue){
        streets.push_back(street);
        avenues.push_back(avenue);
    }
    pair<int, int> cal_MeetingPoint(){
        sort(streets.begin(), streets.end());
        sort(avenues.begin(), avenues.end());
        int midIndex=(streets.size()-1)/2;
        return {streets[midIndex], avenues[midIndex]};
    }
};

int main(){
    int T;
    cin>>T;
    for(int i=0; i<T; i++){
        int S, A, F;
        cin>>S>>A>>F;
        GridCity grid(S, A);
        for(int j=0; j<F; j++){
            int street, avenue;
            cin>>street>>avenue;
            grid.add_Location(street, avenue);
        }
        pair<int, int> meetingPoint=grid.cal_MeetingPoint();
        cout<<"(Street: "<<meetingPoint.first<<", Avenue: "<<meetingPoint.second<<")"<<endl;
    }
    return 0;
}

