#include <stdio.h>

int main(){
	int N,b,c,d;
	scanf("%d", &N);
	b=N%3;
	c=N%7;
	d=N%11;
	if(b==2&&c==5&&d==7) printf("YES");
	else printf("NO");
	return 0;
}
