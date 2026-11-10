#include <stdio.h>

int main(){
	int a;
	float b;
	scanf("%d", &a);
	if(a<=200) b = a*1.2;
	else b = 200*1.2+(a-200)*1.2*0.75;
	printf("%.f", b);
	return 0;
}
