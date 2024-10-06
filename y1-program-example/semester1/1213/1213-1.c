#include <stdio.h>

int max(int num1, int num2){
	int result;
	if (num1 > num2) result = num1;
	else result = num2;
	return result; 
}

void fun(){
	printf("s'dlkfsd'lkg");
}

int main(){
	int a, b;
	scanf("%d%d", &a, &b);
	printf("%d", max(a, b));
	return 123;
}
