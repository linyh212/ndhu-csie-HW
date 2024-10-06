#include <stdio.h>

int main(){
	char str[1024], *p;
	int len;
	p = str;
	//fgets(str, 1024, stdin);
	gets(str);
	while(*p != '\0'){
		p ++;
	}
	printf("%d\n", p - str);
	for(len = 0;*(str + len) != '\0';len ++);
	printf("%d\n", len);
}
