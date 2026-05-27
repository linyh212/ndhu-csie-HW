#include <stdio.h>

int main(){
	freopen("1220.txt", "r", stdin);
	char ch;
	while(scanf("%c", &ch) != EOF)
		printf("%c", ch);
}
