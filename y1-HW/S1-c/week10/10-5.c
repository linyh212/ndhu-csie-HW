#include <stdio.h>
#include <string.h>

char arr[10][10];
char used[10][10];
int find(int i, int j){
	if(i<0||j<0||i>=9||j>=9) return 0;
	if(arr[i][j]=='O') return 1;
	if(arr[i][j]=='X') return 2;
	if(used[i][j]) return 0;
	int flag=0;
	used[i][j]=1;
	flag|=find(i+1, j);
	flag|=find(i-1, j);
	flag|=find(i, j+1);
	flag|=find(i, j-1);
	return flag;
}
int main(){
	int n;
	scanf("%d", &n);
	for(int m=0; m<n; m++){
		for(int i=0; i<9; i++){
			scanf("%s", arr[i]);
		}
		int w=0, b=0;
		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				if(arr[i][j]=='.'){
					memset(used, 0, sizeof(used));
					int flag=find(i, j);
					if(flag==1) w++;
					if(flag==2) b++;
				}
				else if(arr[i][j] == 'O') w++;
				else b++;
			}
		}
		printf("Black %d White %d\n", b, w);
	}
	return 0;
}
