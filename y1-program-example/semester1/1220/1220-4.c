#include <stdio.h>

int main(){
	FILE *file;
	char ch;
	file = fopen("test.txt", "r");
	while(fscanf(file, "%c", &ch) != EOF)
		printf("%c", ch);
	fclose(file);
}
