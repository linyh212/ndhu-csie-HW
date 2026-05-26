#include <stdio.h>

int main(){
	int a, b, c;
	scanf("%d\n%d\n%d",&a,&b,&c);
	int sum=a+b+c;
	float avg=sum/3.0;	
	printf("%.6f\n",avg);
	return 0;
}