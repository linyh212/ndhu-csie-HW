#include <stdio.h>
#include <string.h>

int main(){
	char array[1000000];
	while(fgets (array, sizeof(array), stdin)!=NULL){
		char *str= array;
		int length = strlen(str);
		for(int i=0; i<length; i++){
			if(str[0]<='Z'&&str[0]>='A') break;
			if(str[i]<='z'&&str[i]>='a'){
				str[i]-=32;
				break;
			}
		}
		for(int i=0; i<length; i++){
			if((str[i]=='.'||str[i]=='?'||str[i]=='!')&&str[i+2]<='z'&&str[i+2]>='a') str[i+2]-=32;
		}
		printf("%s", array);
	}
	return 0;
}
