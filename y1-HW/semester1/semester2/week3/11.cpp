#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Digit { // class Digit represent a digit in base 10
  int digit;
public:
  Digit(int d) : digit(d % 10) {}
  Digit():digit(0){}
  void setDigit(int d) { digit = d % 10; }
  int getDigit() const {return digit;}
};
class Integer { //class Integer represent a poistive integer
  Digit value[1000];
  int count;
public:
  //construct an integer in base 10 using a string.
  //This constructor will convert the given string to integer if possible.
  //Otherwise it will convert the digit part in prefix of the given string.
  Integer(string n);
  //default constructor will construct an Integer object with value 0.
  Integer();
  //Display the value in given base from 2 to 16.
  //Order of symbol as 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F.
  void displayInBase(int base) const ;
};
Integer::Integer(string n){
	int length=n.length();
	count=0;
	for(int i=0; i<length; i++){
		if(n[i]<='9'&&n[i]>='0') value[count++].setDigit(n[i]-'0');
		else break;
	}
}
Integer::Integer(){
	count=1;
	value[0].setDigit(0);
}
void Integer::displayInBase(int base) const{
    vector<int> result;
    vector<int> digits(count);
    for(int i=0; i<count; i++){
        digits[i]=value[count-i-1].getDigit();
    }
    while(!digits.empty()){
        int rem=0;
        for(int i=digits.size()-1; i>=0; i--){
            int current=rem*10+digits[i];
            digits[i]=current/base;
            rem=current%base;
        }
        result.push_back(rem);
        while(!digits.empty()&&digits.back()==0){
            digits.pop_back();
        }
    }
    for(int i=result.size()-1; i>=0; i--){
        if(result[i]<10) cout<<result[i];
        else cout<<char('A'+result[i]-10);
    }
}

int main()
{
  string in;
  int j;
  cin >> in;
  Integer i(in);
  for(j = 2;j <= 16;j ++) {
    i.displayInBase(j);
    cout << endl;
  }
}
