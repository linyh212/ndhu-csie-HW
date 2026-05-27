#include <stdio.h>

int main(){
	int arr[5], *p, sum;
	for(p = arr, sum = 0;p < arr + 5;p ++){
		scanf("%d", p);
		sum+= *p;
	}
	for(p = arr;p < arr + 5;p ++){
		printf("%d ", *p);	
	}
	printf("\n%d", sum);
}
