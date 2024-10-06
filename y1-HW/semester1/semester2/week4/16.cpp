#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

class Anagram : public string {
public:
	Anagram(const string& str): string(str){}
	bool operator==(const Anagram &str) const{
		string str1=*this;
		string str2=str;
		transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
		transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
		vector<int> a(26), b(26);
		for(int i=0; i<str1.length(); i++){
			char c=str1[i];
			if(isalpha(c)) a[c-'a']++;
		}
		for(int i=0; i<str2.length(); i++){
			char c=str2[i];
			if(isalpha(c)) b[c-'a']++;
		}
		return a==b;
	}
};

int main() {
	string buf;
	getline(cin, buf);
	Anagram a1(buf);
	getline(cin, buf);
	Anagram a2(buf);
	cout << (a1 == a2) << endl;
	cout << (a2 == a1) << endl;
}
