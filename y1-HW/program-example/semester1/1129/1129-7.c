#include <stdio.h>

int main(){
	char str[1024], copy[1024];
	char *p1 = str, *p2 = copy;
	gets(str);
	while(*p1 != '\0'){
		*p2 = *p1;
		p1 ++;
		p2 ++;
	}
	*p2 = '\0';
	puts(copy);
}
