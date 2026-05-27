#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b){
	int c =*(int *)a;
	int d =*(int *)b;
	if (c>d) return 1;
	if (c<d) return -1;
}

int main(){
	int n;
	scanf("%d",&n);
	int array[n];
	for(int a=0; a<n; a++){
		scanf("%d",&array[a]);
	}
	qsort(array, n, sizeof(int), compare);
	for(int i=n-1; i>=0; i--){
		int t=0;
		for(int j=1; j<=array[n-1]; j++){
			if(array[i]%j==0) t++;
		}
		if(t==2&&array[i]!=1) printf("%d ",array[i]);
	}
	for(int i=0; i<n; i++){
		int t=0;
		for(int j=2; j<=array[n-1]/2; j++){
			if(array[i]%j==0&&array[i]!=j) t++;
			if(t>0||array[i]==0||array[i]==1) printf("%d ",array[i]);
		}
	}
	return 0;
}
