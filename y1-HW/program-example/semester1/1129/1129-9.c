#include <stdio.h>

int main(){
	char str[1024];
	gets(str);
	char *p = str;
	for(;*p != '\0';p ++);
	p --;
	for(;p >= str; p --)
		printf("%c", *p);
}
