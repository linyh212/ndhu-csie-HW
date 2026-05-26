#include <stdio.h>
#include <string.h>

int main(){
	int a, b;
	while(scanf("%d %d",&a,&b)==2){
		int arr[a][b];
		for(int i=0; i<a; i++){
			for(int j=0; j<b; j++){
				scanf("%d",&arr[i][j]);
			}
		}
		int x=0, y=0;
		for(int j=0; j<b; j++){
			for(int i=y; i<b; i++){
				printf("%d ", arr[x][i]);
			}
			if(x+1==a) break;
			x++;
			for(int i=x; i<a; i++){
				printf("%d ", arr[i][b-1]);
			}
			if(y+1==b) break;
			b--;
			for(int i=b-1; i>=y; i--){
				printf("%d ", arr[a-1][i]);
			}
			if(a-1==x) break;
			a--;
			for(int i=a-1; i>=x; i--){
				printf("%d ", arr[i][y]);
			}
			if(b-1==y) break;
			y++;
		}
	}
}
