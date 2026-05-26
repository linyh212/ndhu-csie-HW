#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Digit { // class Digit represent a digit in base 10
  int digit;
public:
  Digit(int d = 0) : digit(d % 10) {}
  void setDigit(int d) { digit = d % 10; }
  int getDigit() const {return digit;}
};

class Integer { //class Integer represent a integer
  Digit value[1000];
  bool sign;
  int count;
public:
  //construct an integer in base 10 using a string.
  //This constructor will convert the given string to integer if possible.
  //Otherwise it will convert the digit part in prefix of the given string.
  Integer(string n);
  //default constructor will construct an Integer object with value 0.
  Integer();
  //function addition will add up two integer and return a new Integer object as result.
  Integer addition(const Integer &b) const;
 //function subtraction will minus b from current object and return a new Integer object as result.
  Integer subtraction(const Integer &b) const;
  void display() const;
};
Integer::Integer(string n){
        reverse(n.begin(), n.end());
        count=0;
        sign=false;
        for(int i=0; i<n.length(); i++){
            if(n[i]>='0'&&n[i]<='9') value[count++].setDigit(n[i]-'0');
            if(n[i]=='-') sign=true;
        }    
};
Integer::Integer(){
	count=1;
	value[0]=Digit(0);
	sign=false;
};
Integer Integer::addition(const Integer &b) const{
	string result="";
	int length=max(count, b.count), carry=0;
	for(int i=0; i<length; i++){
		int temp=value[i].getDigit()+b.value[i].getDigit()+carry;
		carry=0;
		if(temp>=10){
			temp-=10;
			carry=1;
		}
		result.push_back('0'+temp);
	}
	if(carry) result.push_back('1');
	reverse(result.begin(), result.end());
	return Integer(result);
};
Integer Integer::subtraction(const Integer &b) const{
	string result="";
	bool negative=false, carry=false;
	if(count<b.count) negative=true;
	else if(count==b.count){
		for(int i=count-1; i>=0; i--){
			if(value[i].getDigit()<b.value[i].getDigit()){
				negative=true;
				break;
			}
		}
	}
	int length=max(count, b.count), temp;
	for(int i=0; i<length; i++){
		if(!negative) temp=value[i].getDigit()-b.value[i].getDigit()-carry;
		else temp=b.value[i].getDigit()-value[i].getDigit()-carry;
		carry=false;
		if(temp<0){
			temp+=10;
			carry=true;
		}
		result.push_back('0'+temp);
	}
	if(negative) result.push_back('-');
	reverse(result.begin(), result.end());
	return Integer(result);
	};
void Integer::display() const{
	bool is_zero=true;
	if(sign) cout<<"-";
	for(int i=count-1; i>=0; i--){
		if(value[i].getDigit()!=0) is_zero=false;
		if(value[i].getDigit()!=0||!is_zero||i==0) cout<<static_cast<char>('0'+value[i].getDigit());
	}
}

int main() {
  string a, b;
  cin >> a >> b;
  Integer num1(a), num2(b);
  num1.addition(num2).display();
  cout << endl;
  num2.addition(num1).display();
  cout << endl;
  num1.subtraction(num2).display();
  cout << endl;
  num2.subtraction(num1).display();
  cout << endl;
}

