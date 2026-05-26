#include <stdio.h>
 
int main(){
	int n, a, b, c, d;
	scanf("%d", &n);
	a=n%10;
	b=(n/10)%10;
	c=(n/100)%10;
	d=(n/1000)%10;
	if(a==4||b==4||c==4||d==4) printf("Yes");
	else printf("No");
	return 0;
}
