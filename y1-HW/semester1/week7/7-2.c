#include <stdio.h>
#include <string.h>

int main(){
	int n;
	while(scanf("%d", &n)!=EOF){
		int arr[n][2];
		int stack[n][n];
		int top[n];
		for(int i=0; i<n; i++){
			arr[i][0]=i;
			arr[i][1]=0;
			stack[i][0]=i;
			top[i]=1;
		}
		char act1[5], act2[5];
		while(scanf("%s", act1)&&strcmp(act1, "quit")!=0){
			int a, b;
			scanf("%d%s%d", &a, act2, &b);
			int a_x=arr[a][0];
			int a_y=arr[a][1];
			int b_x=arr[b][0];
			int b_y=arr[b][1];
			if(a_x==b_x) continue;
			if(strcmp(act1, "move")==0){
				for(int i=a_y+1; i<top[a_x]; i++){
					arr[stack[a_x][i]][0]=stack[a_x][i];
					arr[stack[a_x][i]][1]=top[stack[a_x][i]];
					stack[stack[a_x][i]][top[stack[a_x][i]]++]=stack[a_x][i];
				}
				top[a_x]=a_y+1;
			}
			if(strcmp(act2, "onto")==0){
				for(int i=b_y+1; i<top[b_x]; i++){
					arr[stack[b_x][i]][0]=stack[b_x][i];
					arr[stack[b_x][i]][1]=top[stack[b_x][i]];
					stack[stack[b_x][i]][top[stack[b_x][i]]++]=stack[b_x][i];
				}
				top[b_x]=b_y+1;
			}
			for(int i=a_y; i<top[a_x]; i++){
				arr[stack[a_x][i]][0]=b_x;
				arr[stack[a_x][i]][1]=top[b_x];
				stack[b_x][top[b_x]++]=stack[a_x][i];
			}
			top[a_x]=a_y;
		}
		for(int i=0; i<n; i++){
			printf("%d:", i);
			for(int j=0; j<top[i]; j++){
				printf(" %d", stack[i][j]);
			}
			printf("\n");
		}
	}
	return 0;
}
