#include <bits/stdc++.h>
using namespace std;

string toLower(const string &str){
	string lowerstr=str;
	transform(lowerstr.begin(), lowerstr.end(), lowerstr.begin(), ::tolower);
	return lowerstr;
}

int main(){
	string inputFile, wordHistFile, charFreqFile;
	cin>>inputFile>>wordHistFile>>charFreqFile;
	
	ifstream inFile(inputFile);
	if(!inFile) return 1;
	map<string, int> wordCount;
	map<char, int> charCount;
	string word;
	while(inFile>>word){
		word=toLower(word);
		wordCount[word]++;
		for(char ch: word){
			if(isalpha(ch)){
				ch=tolower(ch);
				charCount[ch]++;
			}
		}
	}
	inFile.close();
	
	ofstream histFile(wordHistFile);
	if(!histFile) return 1;
	for(const auto &entry: wordCount){
		histFile<<entry.first<<" "<<string(entry.second, '*')<<entry.second<<endl;
	}
	histFile.close();
	
	ofstream freqFile(charFreqFile);
	if(!freqFile) return 1;
	vector<pair<char, int>> sortedCharCount(charCount.begin(), charCount.end());
	sort(sortedCharCount.begin(), sortedCharCount.end(), [](const pair<char, int> &a, const pair<char, int> &b){return a.second<b.second;});
	for(const auto &entry: sortedCharCount){
		freqFile<<entry.first<<" "<<entry.second<<endl;
	}	
	freqFile.close();
	return 0;
}
