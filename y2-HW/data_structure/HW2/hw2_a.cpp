#include <bits/stdc++.h>
using namespace std;

class RailwayBookingSystem{
private:
    map<int, string> station;
    map<int, int> entries;
    map<int, int> exits;
    map<pair<string, string>, int> pairCount;
public:
    void stationsName(const string& filename){
        ifstream file(filename);
        if(!file.is_open()){
            cerr<<"Error opening file: "<<filename<<endl;
            return;
        }
        string line;
        while(getline(file, line)){
            istringstream stream(line);
            int stationID;
            string stationName;
            stream>>stationID>>stationName;
            station[stationID]=stationName;
        }
        file.close();
    }
    void bookingRecords(const string& filename){
        ifstream file(filename);
        if(!file.is_open()){
            cerr<<"Error opening file: "<<filename<<endl;
            return;
        }
        string line;
        while(getline(file, line)){
            istringstream stream(line);
            int startID, endID, numTickets;
            stream>>startID>>endID>>numTickets;
            entries[startID]+=numTickets;
            exits[endID]+=numTickets;
            string start=station.at(startID);
            string end=station.at(endID);
            pair<string, string> pairKey=make_pair(start, end);
            pairCount[pairKey]+=numTickets;
        }
        file.close();
    }
    void summary(const string& filename){
        ofstream file(filename);
        vector<pair<int, int>> sortEntries;
        for(const auto& entry: entries) {
            sortEntries.push_back({entry.first, entry.second});
        }
        sort(sortEntries.begin(), sortEntries.end(), [this](const pair<int, int>& a, const pair<int, int>& b){
            return station.at(a.first)<station.at(b.first);
        });
        for (const auto& entry: sortEntries){
            string staName=station.at(entry.first);
            file<<entry.first<<" "<<staName<<" "<<entry.second<<endl;
        }
        vector<pair<int, int>> sortExits;
        for(const auto& exit: exits){
            sortExits.push_back({exit.first, exit.second});
        }
        sort(sortExits.begin(), sortExits.end(), [](const pair<int, int>& a, const pair<int, int>& b){
            return a.second<b.second;
        });
        for(const auto& exit : sortExits){
            string staName=station.at(exit.first);
            file<<exit.first<<" "<<staName<<" "<<exit.second<<endl;
        }
        vector<pair<pair<string, string>, int>> sortPairs;
        for(const auto& pair: pairCount){
            sortPairs.push_back(pair);
        }
        sort(sortPairs.begin(), sortPairs.end(), [](const pair<pair<string, string>, int>& a, const pair<pair<string, string>, int>& b){
            return a.second<b.second;
        });
        for (const auto& pair: sortPairs){
            string staName1=pair.first.first;
            string staName2=pair.first.second;
            file<<staName1<<" "<<staName2<<" "<<pair.second<<endl;
        }
        file.close();
    }
    map<int, string> getStation() const{
        return station;
    }
};

int main(){
    RailwayBookingSystem railway;
    railway.stationsName("station_name.txt");
    railway.bookingRecords("booking_record.txt");
    railway.summary("summary.txt");
    cout<<"Summary has been written to summary.txt"<<endl;
    return 0;
}
