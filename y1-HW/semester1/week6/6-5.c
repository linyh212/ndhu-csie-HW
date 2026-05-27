#include <stdio.h>

int main(){
	int n, a, b, c, d;
	scanf("%d", &n);
	a=n/1000;
	for(int i=0; i<a; i++){
		printf("M");
	}
	b=(n/100)%10;
	if(b==1)printf("C");
	if(b==2)printf("CC");
	if(b==3)printf("CCC");
	if(b==4)printf("CD");
	if(b==5)printf("D");
	if(b==6)printf("DC");
	if(b==7)printf("DCC");
	if(b==8)printf("DCCC");
	if(b==9)printf("CM");
		c=(n/10)%10;
		if(c==1)printf("X");
		if(c==2)printf("XX");
		if(c==3)printf("XXX");
		if(c==4)printf("XL");
		if(c==5)printf("L");
		if(c==6)printf("LX");
		if(c==7)printf("LXX");
		if(c==8)printf("LXXX");
		if(c==9)printf("XC");
			d=n%10;
			if(d==1)printf("I");
			if(d==2)printf("II");
			if(d==3)printf("III");
			if(d==4)printf("IV");
			if(d==5)printf("V");
			if(d==6)printf("VI");
			if(d==7)printf("VII");
			if(d==8)printf("VIII");
			if(d==9)printf("IX");
}
