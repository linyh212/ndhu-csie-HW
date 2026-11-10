#include <stdio.h>
#include <string.h>

int main(){
	char arr[19][19];
	for(int i=0; i<19; i++){
		for(int j=0; j<19; j++){
			scanf("%c", &arr[i][j]);
		}
	}
	int w=0, b=0, count=0;
	for(int i=0; i<19; i++){
		for(int j=0; j<19; j++){
			//O右
			if(arr[i][j]=='O'&&arr[i][j+1]=='O'&&arr[i][j+2]=='O'&&arr[i][j+3]=='O'&&arr[i][j+4]=='O'){
				w++;
				break;
			}
			//O下
			if(arr[i][j]=='O'&&arr[i+1][j]=='O'&&arr[i+2][j]=='O'&&arr[i+3][j]=='O'&&arr[i+4][j]=='O'){
				w++;
				break;				
			}
			//O右下 
			if(arr[i][j]=='O'&&arr[i+1][j+1]=='O'&&arr[i+2][j+2]=='O'&&arr[i+3][j+3]=='O'&&arr[i+4][j+4]=='O'){
				w++;
				break;
			}
			//O左下
			if(arr[i][j]=='O'&&arr[i+1][j-1]=='O'&&arr[i+2][j-2]=='O'&&arr[i+3][j-3]=='O'&&arr[i+4][j-4]=='O'){
				w++;
				break;
			}
			//X右
			if(arr[i][j]=='X'&&arr[i][j+1]=='X'&&arr[i][j+2]=='X'&&arr[i][j+3]=='X'&&arr[i][j+4]=='X'){
				b++;
				break;
			}
			//X下
			if(arr[i][j]=='X'&&arr[i+1][j]=='X'&&arr[i+2][j]=='X'&&arr[i+3][j]=='X'&&arr[i+4][j]=='X'){
				b++;
				break;
			}
			//X右下
			if(arr[i][j]=='X'&&arr[i+1][j+1]=='X'&&arr[i+2][j+2]=='X'&&arr[i+3][j+3]=='X'&&arr[i+4][j+4]=='X'){
				b++;
				break;
			}
			//X左下
			if(arr[i][j]=='X'&&arr[i+1][j-1]=='X'&&arr[i+2][j-2]=='X'&&arr[i+3][j-3]=='X'&&arr[i+4][j-4]=='X'){
				b++;
				break;
			}
			else count++;
		}
	}
	if(w>0) printf("White");
	if(b>0) printf("Black");
	if(count==361) printf("No winner");
}
