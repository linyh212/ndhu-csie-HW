#include <stdio.h>

int fibonacci(int n){
	if(n == 0 || n == 1) return 1;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(){
	int j;
	for(j = 0;j < 10;j ++)
		printf("%d, %d\n", j, fibonacci(j));
}
