#include <bits/stdc++.h>
using namespace std;

class MIPS{
private:
	string ins, p1, p2, p3;	
public:
	MIPS(string _ins, string _p1, string _p2, string _p3): ins(_ins), p1(_p1), p2(_p2), p3(_p3){
		if(ins=="addi"){
			opr(ins);
			reg(p2);
			reg(p1);
			reg(p3);
		}
		else{
			opr(ins);
			reg(p2);
			reg(p3);
			reg(p1);
			shamt();
			funct(ins);
		}
	}
	void reg(string a){
		string temp=a.substr(0, 3);
		if(temp=="$t0") cout<<"01000";
		else if(temp=="$t1") cout<<"01001";
		else if(temp=="$t2") cout<<"01010";
		else if(temp=="$t3") cout<<"01011";
		else if(temp=="$t4") cout<<"01100";
		else if(temp=="$t5") cout<<"01101";
		else if(temp=="$t6") cout<<"01110";
		else if(temp=="$t7") cout<<"01111";
		else{
			dec_bin(stoi(a));
		}
	}
	void opr(string a){
		if(a=="add") cout<<"000000";
		else if(a=="sub") cout<<"000000";
		else if(a=="or") cout<<"000000";
		else if(a=="and") cout<<"000000";
		else if(a=="addi") cout<<"001000";
	}
	void shamt(){
		cout<<"00000";
	}
	void funct(string a){
		if(a=="add") cout<<"100000";
		else if(a=="sub") cout<<"100010";
		else if(a=="or") cout<<"100101";
		else if(a=="and") cout<<"100100";
	}
	void dec_bin(int a){
		long long digit=0, result=0;
		while(a!=0){
			result+=(a%2)*(pow(10, digit));
			a/=2;
			digit++;
		}
		if(digit==0) digit=1;
		for(int i=0; i<(16-digit); i++) cout<<"0";
		cout<<result;
	}
};

int main(){
	string ins, p1, p2, p3;
	while(cin>>ins>>p1>>p2>>p3){
		MIPS mips(ins, p1, p2, p3);
		cout<<endl;
	}
}
