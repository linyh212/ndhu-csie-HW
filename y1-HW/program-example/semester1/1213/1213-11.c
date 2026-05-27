#include <stdio.h>

int is(char *s1, char *s2){
	int counter1[26] = {}, counter2[26] = {};
	char *j = s1;
	while(*j != '\0'){
		if(*j >= 'a' && *j <= 'z') counter1[*j - 'a'] ++;
		else if(*j >= 'A' && *j <= 'Z') counter1[*j - 'A'] ++;
		j ++;
	}
	j = s2;
	while(*j != '\0'){
		if(*j >= 'a' && *j <= 'z') counter2[*j - 'a'] ++;
		else if(*j >= 'A' && *j <= 'Z') counter2[*j - 'A'] ++;
		j ++;
	}
	int k;
	for(k = 0;k < 26;k ++)
		if(counter1[k] != counter2[k]) return 0;
	return 1;
}

int main(){
	char str1[1024], str2[1024];
	gets(str1);
	gets(str2);
	printf("%d", is(str1, str2));
}
