#include <stdio.h>
#include <string.h>

int main(){
	char array[1001];
	char *str= array;
	while(fgets (array, 1001, stdin)!=NULL){
		int counts[127] = { 0 };
		int i, j;
		size_t len = strlen(str);
		for(i=0; i<len; i++){
			counts[(int)(str[i])]++;
		}
		for(j=1; j<1001; j++){
			for(i=127; i>31; i--){
				if(counts[i]==j) printf("%d %d\n", i, counts[i]);
			}
		}
	}
	printf("\n");
	return 0;
}
