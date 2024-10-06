#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int compare(const void* a, const void* b){
	int c =*(int *)a;
	int d =*(int *)b;
	if(c>d) return 1;
	if(c<d) return -1;
}

int main(){
	int n;
	while(scanf("%d",&n) == 1){
		int array[n];
		for(int a=0; a<n; a++){
			scanf("%d",&array[a]);
		}
		qsort(array, n, sizeof(int), compare);
		int num1, num2, cnt=0, sum=0;
		num1=array[(n-1)/2];
		num2=array[n/2];
		if(n%2!=0){
			for(int i=0; i<n; i++){
				if(array[i]==num1) cnt++;
			}
			sum=1;
			printf("%d %d %d\n",num1,cnt,sum);
		}
		else{
			for(int i=0; i<n/2; i++){
				if(array[i]==num1) cnt++;
			}
			for(int i=n/2; i<n; i++){
				if(array[i]==num2) cnt++;
			}
			sum=num2-num1+1;
			printf("%d %d %d\n",num1,cnt,sum);
		}
	}
	return 0;
}
