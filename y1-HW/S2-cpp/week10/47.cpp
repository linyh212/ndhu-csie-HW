#include <bits/stdc++.h>
using namespace std;

class Decoder{
public:
	Decoder(){
		newMap();
	}
	string decodeMessage(const string& encodedMessage) const{
        string decodedMessage;
        for(char c: encodedMessage){
            auto it= KBmap.find(c);
            if(it!=KBmap.end()) decodedMessage+=it->second;
            else decodedMessage+=c;
        }
        return decodedMessage;
    }
private:
	unordered_map<char, char> KBmap;
	void newMap(){
		string qwerty[]={
            "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./",
        };
        for(const auto& row: qwerty){
        	for(size_t i=0; i<row.size(); i++){
        		if(i>=2) KBmap[row[i]]=row[i-2];
        		else KBmap[row[i]]=row[i];
			}
		}
		KBmap[' ']=' ';
	}
};

int main(){
    Decoder decoder;
    string encodedMessage;
    while(getline(cin, encodedMessage)){
        cout<<decoder.decodeMessage(encodedMessage)<<endl;
    }
    return 0;
}
