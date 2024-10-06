#include <stdio.h>

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
	printf("swap: %d, %d\n", *a, *b);
}

int main(){
	int a = 1, b = 2;
	swap(&a, &b);
	printf("%d, %d\n", a, b);
}
