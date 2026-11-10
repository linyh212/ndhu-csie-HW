#include <stdio.h>

int path(int arr[100][100], int n, int y, int x){
	if(y<0||y>=n||x<0||x>=n||arr[y][x]==1) return 0;
	if(y==n-1&&x==n-1) return 1;
	arr[y][x]=1;
	if(path(arr, n, y+1, x)||path(arr, n, y-1, x)||path(arr, n, y, x+1)||path(arr, n, y, x-1)) return 1;
	else return 0;
}
int main(){
	int n;
	scanf("%d",&n);
	int arr[100][100];
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			scanf("%d",&arr[i][j]);
		}
	}
	if(path(arr, n, 0, 0)) printf("Yes\n");
	else printf("No");
}
