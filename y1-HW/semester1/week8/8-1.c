#include <stdio.h>
#include <string.h>

int main(){
	int a, b, c;
	while(scanf("%d %d %d",&a,&b,&c)==3){
		int arr[a][b];
		memset(arr, 0, sizeof(arr));
		int l, m ,n;
		for(int i=0; i<c; i++){
			scanf("%d %d %d",&l,&m,&n);
			arr[l][m]=n;
		}
		for(int i=0; i<a; i++){
			for(int j=0; j<b; j++){
				printf("%d ", arr[i][j]);
			}
			printf("\n");	
		}
	}
}
