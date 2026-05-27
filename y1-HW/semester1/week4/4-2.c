#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b){
	int c =*(int *)a;
	int d =*(int *)b;
	if (c>d) return 1;
	if (c<d) return -1;
}

int main(){
	int n, i;
	float sum=0;
	scanf("%d" ,&n);
	if(n>6){
		int array[n];
		for(i=0; i<n; i++){
			array[n]=0;
			scanf("%d", &array[i]);
		}
		qsort(array, n, sizeof(int), compare);
		for(i=3; i<n-3; i++){
			sum += array[i];
		}
		sum /= (n-6);
		printf("%.2f", sum);
	}
	return 0;
}
