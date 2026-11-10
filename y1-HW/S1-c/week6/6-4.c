#include <stdio.h>
#include <string.h>

int main(){
	char array[100000];
	int asc[127], len=0, sub=0, ans[2]={0,0};
	memset(asc, -1, sizeof(asc));
	scanf("%s", array);
	for(int i=0; i<strlen(array); i++){
		if(asc[array[i]]>=sub){
			sub=asc[array[i]]+1;
			asc[array[i]]=i;
		}
		else{
			asc[array[i]]=i;
			if((i-sub+1)>len){
				len=i-sub+1;
				ans[0]=sub;
				ans[1]=i;
			}
		}
	}
	for(int i=ans[0]; i<=ans[1]; i++){
		printf("%c",array[i]);
	}
	return 0;
}
