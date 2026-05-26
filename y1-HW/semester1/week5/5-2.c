#include <stdio.h>
#include <math.h>

int main(){
	int n, m, o;
	int k, l;
	scanf("%d" ,&n);
	for(int i=0; i<n; i++){
		scanf("%d" ,&m);
		int array[m];
		for(int j=0; j<m; j++){
			scanf("%d" ,&array[j]);
		}
		int t=0;
		for(k=m-1; k>0; k--){
			for(l=0; l<k; l++){
				if(array[l]>array[l+1]){
					int temp = array[l];
					array[l] = array[l+1];
					array[l+1] = temp;
					t++;
				}
			}
		}
		printf("Optimal train swapping takes %d swaps.\n", t);
	}
	return 0;
}
