#include <stdio.h>

int isEven1(int n){
	if(n % 2 == 0) return 1;
	else return 0;
}
int isEven2(int n){
	if(n % 2 == 0) return 1;
	return 0;
}

int isEven3(int n){
	return n % 2 == 0;
}

int main(){
	int n;
	scanf("%d", &n);
	printf("isEven(%d) = %d\n", n, isEven1(n));
	printf("isEven(%d) = %d\n", n, isEven2(n));
	printf("isEven(%d) = %d\n", n, isEven3(n));
}
