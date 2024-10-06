#include <stdio.h>

int main(){
	float a,b;
	scanf("%f", &a);		
	b = (int)(a*100)/100.0;
	printf("%0.2f\n%0.2f",a,b);
	return 0;
}
