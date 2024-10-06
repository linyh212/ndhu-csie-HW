#include <stdio.h>
#include <string.h>

int main(){
	int n, m;
	while(scanf("%d%d",&n,&m) && n!=0 && m!=0){
		int array[n];
		for(int a=0; a<n; a++){
			scanf("%d",&array[a]);
		}
		for(int k=n-1; k>0; k--){
			for(int l=0; l<k; l++){
				if(array[l]>array[l+1]){
					int temp = array[l];
					array[l] = array[l+1];
					array[l+1] = temp;
				}
			}
		}
		printf("%d %d\n",n,m);
		for(int i=1-m; i<m; i++){
			for(int j=n-1; j>=0; j--){
				if(array[j]%m==i && array[j]%2!=0) printf("%d\n",array[j]);
			}
			for(int l=0; l<n; l++){
				if(array[l]%m==i && array[l]%2==0) printf("%d\n",array[l]);
			}
		}
	}
	printf("0 0");
	return 0;
}
