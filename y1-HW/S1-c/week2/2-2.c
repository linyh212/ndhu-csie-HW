#include <stdio.h>
 
int main(){
	int n, a, b , c, d, e;
	scanf("%d", &n);
	a=n%10;
	b=(n/10)%10;
	c=(n/100)%10;
	d=(n/1000)%10;
	e=a*1000+b*100+c*10+d;
	if(e>1000) printf("%d", e);
	else printf("0%d", e);
	return 0;
}
