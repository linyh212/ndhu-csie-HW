#include <stdio.h>
#include <string.h>

int main(){
	int n;
	scanf("%d\n", &n);
	char arr[100][100], arr2[100][100];
	memset(arr, 0, sizeof(arr));
	int line=0;
	while(scanf("%s", arr[line])==1) line++;
	memcpy(arr2, arr, sizeof(arr));
	for(int num=0; num<n; num++){
		for(int i=0; i<=line; i++){
			for(int j=0; j<strlen(arr[0]); j++){
				int count=0;
				if(i>0&&j>0&&arr[i-1][j-1]=='1') count++;//左上
				if(i>0&&arr[i-1][j]=='1') count++;//上
				if(i>0&&arr[i-1][j+1]=='1') count++;//右上
				if(j>0&&arr[i][j-1]=='1') count++;//左
				if(arr[i][j+1]=='1') count++;//右
				if(j>0&&arr[i+1][j-1]=='1') count++;//左下
				if(arr[i+1][j]=='1') count++;//下
				if(arr[i+1][j+1]=='1') count++;//右下
					if(count<2&&arr[i][j]=='1') arr2[i][j]='0';
					else if(count>=4&&arr[i][j]=='1') arr2[i][j]='0';
					else if(count==3&&arr[i][j]=='0') arr2[i][j]='1';
			}
		}
		memcpy(arr, arr2, sizeof(arr2));
	}
	for(int i=0; i<line; i++){
		for(int j=0; j<strlen(arr[0]); j++){
			printf("%c", arr[i][j]);
		}
		printf("\n");
	}
}
