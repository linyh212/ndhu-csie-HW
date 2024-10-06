#include <stdio.h>
	
int main(){
	int a,b,c;
	scanf("%d %d %d", &a, &b, &c);
	if(a*a+b*b==c*c&&a+b>c) printf("right triangle");
	else if(a*a+b*b<c*c&&a+b>c) printf("obtuse triangle");
	else if(a*a+b*b>c*c&&a+b>c) printf("acute triangle");
	else printf("can not construct");
}
