#include <stdio.h>

int main(){
	char str1[1024], str2[1024];
	char *p1 = str1, *p2 = str2;
	gets(str1);
	gets(str2);
	while(*p1 != '\0')
		p1 ++;
	while(*p2 != '\0'){
		*p1 = *p2;
		p1 ++;
		p2 ++;
	}
	*p1 = '\0';
	puts(str1);
}
