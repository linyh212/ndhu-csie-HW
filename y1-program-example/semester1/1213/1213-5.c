#include <stdio.h>

int isPrime(int n){
	int j;
	if(n < 2) return 0;
	for(j = 2;j < n;j ++)
		if(n % j == 0) return 0;
	return 1;
}

int main(){
	int j;
	for(j = 0;j < 1000;j ++)
		printf("isPrime(%d) = %d\n", j, isPrime(j));
}
