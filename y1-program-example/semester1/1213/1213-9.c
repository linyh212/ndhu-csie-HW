#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int max(int *arr, int len){
	int m = arr[0];
	int j;
	for(j = 1;j < len;j ++)
		if(arr[j] > m) m = arr[j];
	return m;
}

int main(){
	srand(time(NULL));
	int len = rand() % 9 + 2;
	int *arr, j;
	arr = malloc(sizeof(int) * len);
	for(j = 0;j < len;j ++)
		arr[j] = rand() % 100;
	for(j = 0;j < len;j ++)
		printf("%d ", arr[j]);
	printf("\n");
	printf("%d", max(arr, len));
}
