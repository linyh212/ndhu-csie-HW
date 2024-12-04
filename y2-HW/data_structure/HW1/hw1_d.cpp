#include <bits/stdc++.h>
using namespace std;

int calculate(char o1, char o2, int i1, int i2, int i3){
	int calFirst;
	if(o2=='+') calFirst=i2+i3;
	else if(o2=='-') calFirst=i2-i3;
	else if(o2=='*') calFirst=i2*i3;
	else if(o2=='/'){
		if(i3==0) throw "Division by zero";
		calFirst=i2/i3;
	}
	else throw "The operator is unknown";
	int calFinal;
	if(o1=='+') calFinal=i1+calFirst;
	else if(o1=='-') calFinal=i1-calFirst;
	else if(o1=='*') calFinal=i1*calFirst;
	else if(o1=='/'){
		if(calFirst==0) throw "Division by zero";
		calFinal=i1/calFirst;
	}
	else throw "The operator is unknown";
	return calFinal;
}

/*int main(){
	char o1, o2;
	int i1, i2, i3;
	while(cin>>i1>>o1>>i2>>o2>>i3){
		int result=calculate(o1, o2, i1, i2, i3);
		cout<<result<<endl;
	}
	return 0;
}*/
